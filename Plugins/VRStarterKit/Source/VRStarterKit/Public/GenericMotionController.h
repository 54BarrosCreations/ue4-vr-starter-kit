// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MotionControllerComponent.h"
#include "Components/ArrowComponent.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Runtime/UMG/Public/Components/WidgetComponent.h"
#include "Runtime/UMG/Public/Components/WidgetInteractionComponent.h"
#include "GameFramework/Actor.h"
#include "GenericMotionController.generated.h"

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
	bool bWidgetTraceActive = false;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~Public Functions~~
	
	UFUNCTION(BlueprintCallable, Category = "Generic Motion Controller")
	void HandleInteractionTrace();

	UFUNCTION(BlueprintCallable, Category = "Generic Motion Controller")
	void RenderPreviewBeam(FVector StartPoint, FVector EndPoint);

	UFUNCTION(BlueprintCallable, Category = "Generic Motion Controller")
	void PressInteract();

	
	
};
