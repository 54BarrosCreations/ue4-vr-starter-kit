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
}

// Called when the game starts or when spawned
void AGenericVRPawn::BeginPlay()
{
	Super::BeginPlay();
	if (!bValidateControllerIndices) {
		mLeftControllerIndex = LeftControllerIndex;
		mRightControllerIndex = RightControllerIndex;
	}else GetMotionControllerIndices();
	
}

// Called every frame
void AGenericVRPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bUseMotionControllers) UpdateMotionControllerPositions();
}

// Called to bind functionality to input
void AGenericVRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AGenericVRPawn::GetMotionControllerIndices()
{
	TArray<int32> ControllerIndices;
	USteamVRFunctionLibrary::GetValidTrackedDeviceIds(ESteamVRTrackedDeviceType::Controller, ControllerIndices);
	if (ControllerIndices.IsValidIndex(0)) {
		mLeftControllerIndex = ControllerIndices[0];
	} 

	if (ControllerIndices.IsValidIndex(1)) {
		mRightControllerIndex = ControllerIndices[1];
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

