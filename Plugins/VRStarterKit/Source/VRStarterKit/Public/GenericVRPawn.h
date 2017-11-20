// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/InputCore/Classes/InputCoreTypes.h"
#include "GenericMotionController.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "GameFramework/Pawn.h"
#include "GenericVRPawn.generated.h"

UCLASS()
class VRSTARTERKIT_API AGenericVRPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGenericVRPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~Options~~
	UPROPERTY(VisibleAnywhere, Category = "Generic VR Pawn")
	//If true will prevent execution of begin play from the base class, allowing it to be fully implemented in blueprint
	bool bDoNotExecuteBaseBeginPlay = false;

	UPROPERTY(VisibleAnywhere, Category = "Generic VR Pawn")
	//If true will prevent execution of tick from the base class, allowing it to be fully implemented in blueprint
	bool bDoNotExecuteBaseTick = false;

	UPROPERTY(EditAnywhere, Category = "Generic VR Pawn")
	TSubclassOf<AGenericMotionController> MotionControllerTemplate;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~Component Hierarchy~~ 
	UPROPERTY(VisibleDefaultsOnly, Category = "Generic VR Pawn")
	USceneComponent* DefaultSceneRoot = nullptr;

	UPROPERTY(VisibleDefaultsOnly, Category = "Generic VR Pawn")
	USceneComponent* VROrigin = nullptr;

	UPROPERTY(VisibleDefaultsOnly, Category = "Generic VR Pawn")
	UCameraComponent* DefaultCamera = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Generic VR Pawn")
	AGenericMotionController* LeftController = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Generic VR Pawn")
	AGenericMotionController* RightController = nullptr;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~Variables~~
	UPROPERTY(BlueprintReadOnly, Category = "Generic VR Pawn")
	AGenericMotionController* ActiveController = nullptr;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~Functions~~

	UFUNCTION(BlueprintCallable, Category = "Generic VR Pawn")
	void SpawnMotionController(EControllerHand Hand, bool bInvertScale = false);

	UFUNCTION(BlueprintCallable, Category = "Generic VR Pawn")
	void InitControllerInteraction();

	UFUNCTION(BlueprintCallable, Category = "Generic Motion Controller")
	void SetActiveController(AGenericMotionController* NewActive);


private:
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~Input Functions~~
	UFUNCTION(Category = "Generic VR Pawn")
	void LeftTriggerDown();

	UFUNCTION(Category = "Generic VR Pawn")
	void LeftTriggerUp();

	UFUNCTION(Category = "Generic VR Pawn")
	void RightTriggerDown();

	UFUNCTION(Category = "Generic VR Pawn")
	void RightTriggerUp();

	
};
