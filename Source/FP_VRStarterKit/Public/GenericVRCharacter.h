// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "PickupObject.h"
#include "VRCharacterInteractionComponent.h"
#include "MotionControllerComponent.h"
#include "SteamVRFunctionLibrary.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "HeadMountedDisplay.h"
#include "Runtime/HeadMountedDisplay/Public/HeadMountedDisplayTypes.h"
#include "PickupObject.h"
#include "GameFramework/Character.h"
#include "GenericVRCharacter.generated.h"

UCLASS()
class FP_VRSTARTERKIT_API AGenericVRCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGenericVRCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	EHMDDeviceType::Type HMD;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~VR Character Options~~
	UPROPERTY(EditAnywhere, Category = "Generic VR Character|Motion Controllers")
	bool bUseMotionControllers = true;

	UPROPERTY(EditAnywhere, Category = "Generic VR Character|Motion Controllers")
	bool bUseLaserInteraction = true;

	UPROPERTY(EditAnywhere, Category = "Generic VR Character|Motion Controllers", meta = (EditCondition = "bUseLaserInteraction"))
	float LaserDrawDistance = 5000.f;

	UPROPERTY(EditAnywhere, Category = "Generic VR Character|Motion Controllers")
	bool bAllowGripping = true;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~Inherited Components~~ 
	UPROPERTY(VisibleDefaultsOnly)
	//Camera Component Root, parent mesh object to this
	USceneComponent* VRSceneRoot = nullptr;

	UPROPERTY(VisibleDefaultsOnly)
	//Camera Component Root, parent mesh object to this
	USceneComponent* VROrigin = nullptr;

	UPROPERTY(VisibleDefaultsOnly)
	//Camera Component Root, parent mesh object to this
	UCameraComponent* VRCameraOrigin = nullptr;

	UPROPERTY(VisibleDefaultsOnly)
	UMotionControllerComponent* LeftMotionControllerRoot = nullptr;

	UPROPERTY(VisibleDefaultsOnly)
	UMotionControllerComponent* RightMotionControllerRoot = nullptr;

	UPROPERTY(VisibleDefaultsOnly)
	UParticleSystemComponent* PS_LeftControllerBeam = nullptr;

	UPROPERTY(VisibleDefaultsOnly)
	UParticleSystemComponent* PS_RightControllerBeam = nullptr;

	UPROPERTY(VisibleDefaultsOnly)
	UVRCharacterInteractionComponent* InteractionComponent = nullptr;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~Optional Components~~ 

	USphereComponent* GrabSphere_L = nullptr;
	USphereComponent* GrabSphere_R = nullptr;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~Functions~~ 

	UFUNCTION()
	void SetInitialActiveController();

	UFUNCTION()
	void UpdateMotionControllerPositions();

	UFUNCTION()
	void UpdateLaser();

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~Controller Events~~

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "VR Character Controller Events")
	void LeftMotionControllerTriggerDown(UVRCharacterInteractionComponent* VRInteractionComponent);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "VR Character Controller Events")
	void LeftMotionControllerTriggerUp(UVRCharacterInteractionComponent* VRInteractionComponent);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "VR Character Controller Events")
	void RightMotionControllerTriggerDown(UVRCharacterInteractionComponent* VRInteractionComponent);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "VR Character Controller Events")
	void RightMotionControllerTriggerUp(UVRCharacterInteractionComponent* VRInteractionComponent);

private:

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~Input~~
	void LeftTriggerDown();
	void LeftTriggerUp();
	void RightTriggerDown();
	void RightTriggerUp();
	void LeftGripDown();
	void LeftGripUp();
	void RightGripDown();
	void RightGripUp();

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~Gripping~~
	AActor* GetClosestValidActor(TArray<AActor*> InOverlappingActors);
	AActor* RightGrippedActor = nullptr;
	AActor* LeftGrippedActor = nullptr;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~VR Setup~~

	void InitializeHMD();
	void GetOptionalComponents();
	
};
