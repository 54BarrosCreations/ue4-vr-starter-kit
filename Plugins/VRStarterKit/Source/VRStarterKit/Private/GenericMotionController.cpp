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
		HandleInteractionTracePreview();
	}
}

bool AGenericMotionController::PollForUIHit()
{
	FHitResult Result = InteractionComponent->GetLastHitResult();
	if (Result.bBlockingHit && Result.GetComponent()->IsA<UWidgetComponent>()) return true;
	else return false;
}

void AGenericMotionController::HandleInteractionTracePreview()
{
	if (bControllerActive) {
		if (!TracePreviewParticleEmitter->bVisible)	TracePreviewParticleEmitter->SetVisibility(true);
		InteractionComponent->Activate();
		FHitResult Result = InteractionComponent->GetLastHitResult();
		TracePreviewParticleEmitter->SetBeamSourcePoint(0, Result.TraceStart, 0);
		TracePreviewParticleEmitter->SetBeamEndPoint(0, Result.TraceEnd);
	} else {
		if (TracePreviewParticleEmitter->bVisible)	TracePreviewParticleEmitter->SetVisibility(false);
		if (bFirstTimeSetupComplete) InteractionComponent->Deactivate();
	}	
}

void AGenericMotionController::PressInteract()
{
	InteractionComponent->PressPointerKey(EKeys::LeftMouseButton);
}

void AGenericMotionController::ReleaseInteract()
{
	InteractionComponent->ReleasePointerKey(EKeys::LeftMouseButton);
}


