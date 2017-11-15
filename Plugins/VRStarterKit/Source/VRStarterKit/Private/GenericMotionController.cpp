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

	//Interaction Component
	InteractionComponent = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("Widget Interaction Component"));
	InteractionComponent->SetupAttachment(MotionController);
	//if (InteractionMode == EVRInteractionMode::IM_ALWAYS_OFF)	InteractionComponent->SetComponentTickEnabled(false);

	//Particle System
	TracePreviewParticleEmitter = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle System Component"));
	TracePreviewParticleEmitter->SetupAttachment(InteractionComponent);
}

// Called when the game starts or when spawned
void AGenericMotionController::BeginPlay()
{
	Super::BeginPlay();
	if (!bDoNotRunBaseBeginPlay) {

	}
	
}

// Called every frame
void AGenericMotionController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bDoNotRunBaseTick) {
		HandleInteractionTrace();
	}
}

void AGenericMotionController::HandleInteractionTrace()
{
	FHitResult Result = InteractionComponent->GetLastHitResult();
	TracePreviewParticleEmitter->SetBeamSourcePoint(0, Result.TraceStart, 0);
	TracePreviewParticleEmitter->SetBeamEndPoint(0, Result.TraceEnd);
}

void AGenericMotionController::RenderPreviewBeam(FVector StartPoint, FVector EndPoint)
{
	
}

void AGenericMotionController::PressInteract()
{

}

