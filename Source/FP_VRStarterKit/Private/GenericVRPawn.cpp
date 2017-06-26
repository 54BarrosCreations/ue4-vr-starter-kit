// Fill out your copyright notice in the Description page of Project Settings.

#include "GenericVRPawn.h"

// Sets default values
AGenericVRPawn::AGenericVRPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Set up default component roots
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Default Scene Root"));
	RootComponent = DefaultSceneRoot;
	VROrigin = CreateDefaultSubobject<USceneComponent>(TEXT("VR Origin"));
	VROrigin->SetupAttachment(DefaultSceneRoot);
	DefaultCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Default Camera"));
	DefaultCamera->SetupAttachment(VROrigin);
	LeftControllerRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Left Controller"));
	LeftControllerRoot->SetupAttachment(VROrigin);
	RightControllerRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Right Controller"));
	RightControllerRoot->SetupAttachment(VROrigin);
	InteractionComponent = CreateDefaultSubobject<UVRInteractionComponent>(TEXT("Interaction Component"));
	PS_LeftControllerBeam = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Left Beam"));
	PS_LeftControllerBeam->SetupAttachment(LeftControllerRoot);
	PS_LeftControllerBeam->SetRelativeLocation(FVector::ZeroVector);
	PS_RightControllerBeam = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Right Beam"));
	PS_RightControllerBeam->SetupAttachment(RightControllerRoot);
	PS_RightControllerBeam->SetRelativeLocation(FVector::ZeroVector);
}

// Called when the game starts or when spawned
void AGenericVRPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGenericVRPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bUseMotionControllers) UpdateMotionControllerPositions();
	if (bEnableLaserInput) UpdateLaser();
}

// Called to bind functionality to input
void AGenericVRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("LeftTrigger", EInputEvent::IE_Pressed, this, &AGenericVRPawn::LeftTriggerDown);
	PlayerInputComponent->BindAction("RightTrigger", EInputEvent::IE_Pressed, this, &AGenericVRPawn::RightTriggerDown);
}

void AGenericVRPawn::FirstTimeSetActiveController()
{
	if (!InteractionComponent) return;
	if (InteractionComponent->TraceForUI(LeftControllerRoot)) {
		InteractionComponent->SetRightLaserActive(false);
		InteractionComponent->ControllerFirstTimeActive = false;
	}else if (InteractionComponent->TraceForUI(RightControllerRoot)) {
		InteractionComponent->SetRightLaserActive(true);
		InteractionComponent->ControllerFirstTimeActive = false;
	}
}

void AGenericVRPawn::UpdateMotionControllerPositions()
{
	if (!LeftControllerRoot || !RightControllerRoot) return;

	//Left Controller
	FVector LPos;
	FRotator LRot;
	USteamVRFunctionLibrary::GetHandPositionAndOrientation(0, EControllerHand::Left, LPos, LRot);
	LeftControllerRoot->SetRelativeTransform(FTransform(LRot, LPos, FVector(1, 1, 1)));

	//Right Controller
	FVector RPos;
	FRotator RRot;
	USteamVRFunctionLibrary::GetHandPositionAndOrientation(0, EControllerHand::Right, RPos, RRot);
	RightControllerRoot->SetRelativeTransform(FTransform(RRot, RPos, FVector(1, 1, 1)));
}

void AGenericVRPawn::UpdateLaser()
{
	if (InteractionComponent->ControllerFirstTimeActive) FirstTimeSetActiveController();
	else {
		FHitResult HitResult;
		if (InteractionComponent->RightControllerActive) {
			if (InteractionComponent->TraceForUI(RightControllerRoot, HitResult)) {
				PS_RightControllerBeam->SetBeamSourcePoint(0, RightControllerRoot->GetComponentLocation(), 0);
				PS_RightControllerBeam->SetBeamEndPoint(0, InteractionComponent->GetBeamEndPoint(RightControllerRoot, HitResult));
				if (PS_LeftControllerBeam->bVisible) PS_LeftControllerBeam->SetVisibility(false);
				if (!PS_RightControllerBeam->bVisible) PS_RightControllerBeam->SetVisibility(true);
			}
			else {
				if (PS_RightControllerBeam->bVisible) PS_RightControllerBeam->SetVisibility(false);
			}
		}
		else if (!InteractionComponent->RightControllerActive) {
			if (InteractionComponent->TraceForUI(LeftControllerRoot, HitResult)) {
				PS_LeftControllerBeam->SetBeamSourcePoint(0, LeftControllerRoot->GetComponentLocation(), 0);
				PS_LeftControllerBeam->SetBeamEndPoint(0, InteractionComponent->GetBeamEndPoint(LeftControllerRoot, HitResult));
				if (PS_RightControllerBeam->bVisible) PS_RightControllerBeam->SetVisibility(false);
				if (!PS_LeftControllerBeam->bVisible) PS_LeftControllerBeam->SetVisibility(true);
			}
			else {
				if (PS_LeftControllerBeam->bVisible) PS_LeftControllerBeam->SetVisibility(false);
			}
		}
	}
}

void AGenericVRPawn::LeftTriggerDown()
{
	if (!InteractionComponent) return;
	if (InteractionComponent->TraceForUI(LeftControllerRoot) && InteractionComponent->RightControllerActive) {
		InteractionComponent->SetRightLaserActive(false);
	}
}

void AGenericVRPawn::RightTriggerDown()
{
	if (!InteractionComponent) return;
	if (InteractionComponent->TraceForUI(RightControllerRoot) && !InteractionComponent->RightControllerActive) {
		InteractionComponent->SetRightLaserActive(true);
	}
}

