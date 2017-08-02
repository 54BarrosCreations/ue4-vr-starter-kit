// Fill out your copyright notice in the Description page of Project Settings.

#include "GenericVRCharacter.h"


// Sets default values
AGenericVRCharacter::AGenericVRCharacter(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer.DoNotCreateDefaultSubobject(ACharacter::MeshComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	VRSceneRoot = CreateDefaultSubobject<USceneComponent>(FName("VR Scene Root"));
	RootComponent = VRSceneRoot;
	VROrigin = CreateDefaultSubobject<USceneComponent>(TEXT("VR Origin"));
	VROrigin->SetupAttachment(VRSceneRoot);
	VRCameraOrigin = CreateDefaultSubobject<UCameraComponent>(FName("VR Camera Origin"));
	VRCameraOrigin->SetupAttachment(VROrigin);
	GetCapsuleComponent()->SetupAttachment(VRCameraOrigin);

	//Left Motion Controller
	LeftMotionControllerRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Left Controller"));
	LeftMotionControllerRoot->SetupAttachment(VROrigin);
	LeftMotionControllerRoot->SetIsReplicated(true);
	PS_LeftControllerBeam = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Left Controller Beam"));
	PS_LeftControllerBeam->SetupAttachment(LeftMotionControllerRoot);

	//Right Motion Controller
	RightMotionControllerRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Right Controller"));
	RightMotionControllerRoot->SetupAttachment(VROrigin);
	RightMotionControllerRoot->SetIsReplicated(true);
	PS_RightControllerBeam = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Right Controller Beam"));
	PS_RightControllerBeam->SetupAttachment(RightMotionControllerRoot);

	InteractionComponent = CreateDefaultSubobject<UVRCharacterInteractionComponent>(TEXT("Interaction Component"));
}

// Called when the game starts or when spawned
void AGenericVRCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGenericVRCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsLocallyControlled())	UpdateMotionControllerPositions();
	if (bUseLaserInteraction)	UpdateLaser();
}

// Called to bind functionality to input
void AGenericVRCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AGenericVRCharacter::SetInitialActiveController()
{
	if (!InteractionComponent) return;
	FHitResult HitResult;
	if (InteractionComponent->TraceForUI(LeftMotionControllerRoot, HitResult)) {
		InteractionComponent->bRightControllerActive = false;
		InteractionComponent->bControllerFirstTimeActive = false;
	}
	else if (InteractionComponent->TraceForUI(RightMotionControllerRoot, HitResult)) {
		InteractionComponent->bRightControllerActive = true;
		InteractionComponent->bControllerFirstTimeActive = false;
	}
}

void AGenericVRCharacter::UpdateMotionControllerPositions()
{
	//Left Controller
	FVector LPos;
	FRotator LRot;
	USteamVRFunctionLibrary::GetHandPositionAndOrientation(0, EControllerHand::Left, LPos, LRot);
	LeftMotionControllerRoot->SetRelativeTransform(FTransform(LRot, LPos, FVector(1, 1, 1)));

	//Right Controller
	FVector RPos;
	FRotator RRot;
	USteamVRFunctionLibrary::GetHandPositionAndOrientation(0, EControllerHand::Right, RPos, RRot);
	RightMotionControllerRoot->SetRelativeTransform(FTransform(RRot, RPos, FVector(1, 1, 1)));
}

void AGenericVRCharacter::UpdateLaser()
{
	if (InteractionComponent->bControllerFirstTimeActive) SetInitialActiveController();
	else {
		FHitResult HitResult;
		if (InteractionComponent->bRightControllerActive) {
			if (InteractionComponent->TraceForUI(RightMotionControllerRoot, HitResult)) {
				PS_RightControllerBeam->SetBeamSourcePoint(0, RightMotionControllerRoot->GetComponentLocation(), 0);
				PS_RightControllerBeam->SetBeamEndPoint(0, InteractionComponent->GetLaserBeamEndPoint(RightMotionControllerRoot, HitResult));
				if (PS_LeftControllerBeam->bVisible) PS_LeftControllerBeam->SetVisibility(false);
				if (!PS_RightControllerBeam->bVisible) PS_RightControllerBeam->SetVisibility(true);
			}
			else {
				if (PS_RightControllerBeam->bVisible) PS_RightControllerBeam->SetVisibility(false);
			}
		}
		else if (!InteractionComponent->bRightControllerActive) {
			if (InteractionComponent->TraceForUI(LeftMotionControllerRoot, HitResult)) {
				PS_LeftControllerBeam->SetBeamSourcePoint(0, LeftMotionControllerRoot->GetComponentLocation(), 0);
				PS_LeftControllerBeam->SetBeamEndPoint(0, InteractionComponent->GetLaserBeamEndPoint(LeftMotionControllerRoot, HitResult));
				if (PS_RightControllerBeam->bVisible) PS_RightControllerBeam->SetVisibility(false);
				if (!PS_LeftControllerBeam->bVisible) PS_LeftControllerBeam->SetVisibility(true);
			}
			else {
				if (PS_LeftControllerBeam->bVisible) PS_LeftControllerBeam->SetVisibility(false);
			}
		}
	}
}

