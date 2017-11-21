// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TeleportPreviewActor.h"
#include "MotionControllerComponent.h"
#include "Components/ArrowComponent.h"
#include "Runtime/Engine/Classes/AI/Navigation/NavigationSystem.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStaticsTypes.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Runtime/UMG/Public/Components/WidgetComponent.h"
#include "Runtime/UMG/Public/Components/WidgetInteractionComponent.h"
#include "GameFramework/Actor.h"
#include "GenericMotionController.generated.h"

class AGenericVRPawn;

UCLASS()
class VRSTARTERKIT_API AGenericMotionController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGenericMotionController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~Options~~
	UPROPERTY(EditAnywhere, Category = "Generic Motion Controller")
	//If true will prevent execution of begin play from the base class, allowing it to be fully implemented in blueprint
	bool bDoNotRunBaseBeginPlay = false;

	UPROPERTY(EditAnywhere, Category = "Generic Motion Controller")
	//If true will prevent execution of tick from the base class, allowing it to be fully implemented in blueprint
	bool bDoNotRunBaseTick = false;

	UPROPERTY(EditAnywhere, Category = "Generic Motion Controller")
	bool bReplicateMotionToServer = false;

	UPROPERTY(EditAnywhere, Category = "Generic Motion Controller: Locomotion")
	TSubclassOf<AActor> TeleportPreviewActorTemplate;

	UPROPERTY(EditAnywhere, Category = "Generic Motion Controller: Locomotion")
	//Types of objects that will block teleport traces
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectsToTrace;

	UPROPERTY(EditAnywhere, Category = "Generic Motion Controller: Locomotion")
	float TeleportLaunchVelocity = 5000.f;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~Component Hierarchy~~

	UPROPERTY(VisibleDefaultsOnly, Category = "Generic Motion Controller")
	USceneComponent* DefaultSceneRoot = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Generic Motion Controller")
	UMotionControllerComponent* MotionController = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Generic Motion Controller")
	UWidgetInteractionComponent* InteractionComponent = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Generic Motion Controller")
	UParticleSystemComponent* TracePreviewParticleEmitter = nullptr;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~Public Variables~~

	UPROPERTY(BlueprintReadOnly, Category = "Generic Motion Controller")
	bool bControllerActive = false;

	UPROPERTY(BlueprintReadOnly, Category = "Generic Motion Controller")
	bool bFirstTimeSetupComplete = false;

	UFUNCTION(BlueprintCallable, Category = "Generic Motion Controller")
	bool PollForUIHit();

	UPROPERTY(BlueprintReadWrite, Category = "Generic Motion Controller")
	AActor* TeleportPreviewActor = nullptr;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~Public Functions~~
	
	UFUNCTION(BlueprintCallable, Category = "Generic Motion Controller")
	void HandleInteractionTracePreview();

	UFUNCTION(BlueprintCallable, Category = "Generic Motion Controller")
	void PressInteract();

	UFUNCTION(BlueprintCallable, Category = "Generic Motion Controller")
	void ReleaseInteract();

	UFUNCTION(BlueprintCallable, Category = "Generic Motion Controller: Locomotion")
	bool TraceTeleportDestination(FVector &outNavLocation, FVector &outTraceLocation, TArray<FVector> &outTracePoints, TArray<AActor*> ActorsToIgnore);

	UFUNCTION(BlueprintCallable, Category = "Generic Motion Controller: Locomotion")
	void ActivateTeleporter();

private:
	AGenericVRPawn* ParentPawn = nullptr;
	void GetParentPawn();
	
};
