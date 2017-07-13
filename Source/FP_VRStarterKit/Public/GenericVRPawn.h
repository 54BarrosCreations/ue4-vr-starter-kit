// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "VRInteractionComponent.h"
#include "SteamVRFunctionLibrary.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "HeadMountedDisplay.h"
#include "GameFramework/Pawn.h"
#include "GenericVRPawn.generated.h"

UCLASS()
class FP_VRSTARTERKIT_API AGenericVRPawn : public APawn
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
	//~~Pawn Options~~

	UPROPERTY(EditAnywhere, Category = "Generic VR Pawn|Motion Controllers")
	//If true, will allow the motion controllers to tick. Select false to use gamepad.
	bool bUseMotionControllers = true;

	UPROPERTY(EditAnywhere, Category = "Generic VR Pawn|Motion Controllers")
	//If true, will enable laser pointer input
	bool bEnableLaserInput = true;

	UPROPERTY(EditAnywhere, Category = "Generic VR Pawn|Motion Controllers|Controller Options", meta = (EditCondition = "bUseMotionControllers"))
	//If true, Left Controller will be enabled.
	bool bUseLeftController = true;

	UPROPERTY(EditAnywhere, Category = "Generic VR Pawn|Motion Controllers|Controller Options", meta = (EditCondition = "bUseMotionControllers"))
	//If true, Right Controller will be enabled.
	bool bUseRightController = true;

	UPROPERTY(EditAnywhere, Category = "Generic VR Pawn|Motion Controllers|Laser Pointer", meta = (EditCondition = "bLaserPointer"))
	//How far forward to check for laser collisions
	float LaserDrawDistance = 1000.f;


	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~Inherited Components~~ 
	
	UPROPERTY(VisibleDefaultsOnly)
	//Default root for the pawn
	USceneComponent* DefaultSceneRoot = nullptr;

	UPROPERTY(VisibleDefaultsOnly)
	//Root Component for Default Camera and Pawn Mesh
	USceneComponent* VROrigin = nullptr;

	UPROPERTY(VisibleDefaultsOnly)
	//Camera Component
	UCameraComponent* DefaultCamera = nullptr;

	UPROPERTY(VisibleDefaultsOnly)
	//Root Component for Left Motion Controller Mesh
	USceneComponent* LeftControllerRoot = nullptr;

	UPROPERTY(VisibleDefaultsOnly)
	//Root Component for Right Motion Controller Mesh
	USceneComponent* RightControllerRoot = nullptr;

	UPROPERTY(VisibleDefaultsOnly)
	//Controls ui interaction
	UVRInteractionComponent* InteractionComponent = nullptr;

	UPROPERTY(VisibleDefaultsOnly)
	//Particle Emitter for the Left Laser Beam, only visible if using laser interaction
	UParticleSystemComponent* PS_LeftControllerBeam = nullptr;

	UPROPERTY(VisibleDefaultsOnly)
	//Particle Emitter for the Right Laser Beam, only visible if using laser interaction
	UParticleSystemComponent* PS_RightControllerBeam = nullptr;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~Functions~~

	UFUNCTION(BlueprintCallable, Category = "Generic VR Pawn Functions")
	void FirstTimeSetActiveController();

	UFUNCTION(NetMulticast, Reliable, Category ="Generic VR Pawn Functions")
	//Update postion of motion controllers
	void UpdateMotionControllerPositions();

private:

	//Update Laser Status
	void UpdateLaser();

	UFUNCTION()
	void LeftTriggerDown();

	UFUNCTION()
	void RightTriggerDown();

	







	
	
};
