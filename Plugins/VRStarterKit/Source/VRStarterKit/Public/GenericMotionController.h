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

UENUM(BlueprintType)
enum class EVRInteractionMode : uint8 {
	IM_SHOW_ON_WIDGETS_ONLY UMETA(DisplayName = "Only Active Over Widgets"),
	IM_ALWAYS_OFF UMETA(DisplayName = "Off")
};

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
	EVRInteractionMode InteractionMode = EVRInteractionMode::IM_ALWAYS_OFF;

	UPROPERTY(EditAnywhere, Category = "Generic Motion Controller")
	bool bReplicateMotionToServer = false;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~Component Hierarchy~~

	UPROPERTY(VisibleDefaultsOnly)
	USceneComponent* DefaultSceneRoot = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	UMotionControllerComponent* MotionController = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	UWidgetInteractionComponent* InteractionComponent = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	UArrowComponent* ArrowComponent = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
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
	void Interact();

	
	
};
