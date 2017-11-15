// Fill out your copyright notice in the Description page of Project Settings.

#include "GenericVRPawn.h"


// Sets default values
AGenericVRPawn::AGenericVRPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Default Scene Root"));
	RootComponent = DefaultSceneRoot;
	VROrigin = CreateDefaultSubobject<USceneComponent>(TEXT("VR Origin"));
	VROrigin->SetupAttachment(DefaultSceneRoot);
	DefaultCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Default Camera"));
	DefaultCamera->SetupAttachment(VROrigin);
}

// Called when the game starts or when spawned
void AGenericVRPawn::BeginPlay()
{
	Super::BeginPlay();
	if (!bDoNotExecuteBaseBeginPlay) {
		if (!LeftController) SpawnMotionController(EControllerHand::Left);
		if (!RightController) SpawnMotionController(EControllerHand::Right);
	}
}

// Called every frame
void AGenericVRPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGenericVRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AGenericVRPawn::SpawnMotionController(EControllerHand Hand, bool bInvertScale)
{
	//Return if no Motion Controller Template
	if (!MotionControllerTemplate->IsValidLowLevel()) {
		UE_LOG(LogTemp, Error, TEXT("Motion Controller Template is not set. Cannot spawn motion controllers."));
		return;
	}

	//Spawn Controller
	FActorSpawnParameters Params;
	Params.Owner = this;
	FTransform SpawnTransform = FTransform::Identity;
	auto a = GetWorld()->SpawnActor<AGenericMotionController>(MotionControllerTemplate, SpawnTransform, Params);
	if (bInvertScale) a->SetActorScale3D(a->GetActorScale3D() * -1);
	if (Hand == EControllerHand::Left) {
		LeftController = a;
		LeftController->MotionController->Hand = EControllerHand::Left;
	} else if (Hand == EControllerHand::Right) {
		RightController = a;
		RightController->MotionController->Hand = EControllerHand::Right;
	} else {
		UE_LOG(LogActor, Error, TEXT("Invalid Hand type. Spawn controller will be marked for kill."));
		a->Destroy();
	}

	if (!a) return;

	//Attach Controller to Pawn
	FAttachmentTransformRules AttachRules = FAttachmentTransformRules::SnapToTargetNotIncludingScale;
	a->AttachToComponent(VROrigin, AttachRules);
}

void AGenericVRPawn::LeftTriggerDown()
{
}

void AGenericVRPawn::LeftTriggerUp()
{
}

void AGenericVRPawn::RightTriggerDown()
{
}

void AGenericVRPawn::RightTriggerUp()
{
}

