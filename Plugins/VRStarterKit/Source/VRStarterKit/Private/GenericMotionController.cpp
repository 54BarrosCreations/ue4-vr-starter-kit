// Fill out your copyright notice in the Description page of Project Settings.

#include "GenericMotionController.h"
#include "GenericVRPawn.h"


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
		GetParentPawn();
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

bool AGenericMotionController::TraceTeleportDestination(FVector & outNavLocation, FVector & outTraceLocation, TArray<FVector>& outTracePoints, TArray<AActor*> ActorsToIgnore)
{
	FPredictProjectilePathParams ProjectileParams;
	ProjectileParams.StartLocation = InteractionComponent->GetForwardVector();
	ProjectileParams.LaunchVelocity = InteractionComponent->GetForwardVector() * TeleportLaunchVelocity;
	ProjectileParams.bTraceWithCollision = true;
	ProjectileParams.ProjectileRadius = 0.f;
	ProjectileParams.ActorsToIgnore = ActorsToIgnore;
	ProjectileParams.ActorsToIgnore.Add(GetOwner());
	ProjectileParams.ObjectTypes = ObjectsToTrace;
	FPredictProjectilePathResult Result;
	if (UGameplayStatics::PredictProjectilePath(this, ProjectileParams, Result)) {
		auto Nav = GetWorld()->GetNavigationSystem();
		if (!Nav) return false;
		FVector NavExtent = FVector(500.f, 500.f, 500.f);
		FVector OutExtent = Nav->ProjectPointToNavigation(this, Result.HitResult.ImpactPoint, (ANavigationData*)0, 0, NavExtent);
		if (!Result.HitResult.Location.Equals(OutExtent, 0.0100)) {
			outNavLocation = OutExtent;
			outTraceLocation = Result.HitResult.ImpactPoint;
			for (FPredictProjectilePathPointData d : Result.PathData) outTracePoints.Add(d.Location);
			return true;
		}
	}

	return false;
}

void AGenericMotionController::ActivateTeleporter()
{
	if (!TeleportPreviewActor) {
		
	}
}

void AGenericMotionController::GetParentPawn()
{
	auto p = GetParentActor();
	if (p->IsA<AGenericVRPawn>()) {
		ParentPawn = Cast<AGenericVRPawn>(p);
	} else UE_LOG(LogTemp, Error, TEXT("Parent actor is not of type AGenericVRPawn. Many motion controller functions will not work."));
}


