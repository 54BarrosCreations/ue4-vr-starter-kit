// Fill out your copyright notice in the Description page of Project Settings.

#include "GenericMotionController.h"


// Sets default values
AGenericMotionController::AGenericMotionController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Default Scene Root"));
	RootComponent = DefaultSceneRoot;
	MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Motion Controller Component"));
	MotionController->SetupAttachment(DefaultSceneRoot);
	MotionController->bDisableLowLatencyUpdate = true;
}

// Called when the game starts or when spawned
void AGenericMotionController::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void AGenericMotionController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

