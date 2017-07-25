// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "MotionControllerComponent.h"
#include "VRInteractionComponent.h"
#include "SteamVRFunctionLibrary.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "HeadMountedDisplay.h"
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

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~VR Character Options~~
	UPROPERTY(EditAnywhere, Category = "Generic VR Character|Motion Controllers")
	bool bUseMotionControllers = true;

	UPROPERTY(EditAnywhere, Category = "Generic VR Character|Motion Controllers")
	bool bUseLaserInteraction = true;

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
	USceneComponent* LeftMotionControllerRoot = nullptr;

	UPROPERTY(VisibleDefaultsOnly)
	USceneComponent* RightMotionControllerRoot = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	UVRInteractionComponent* InteractionComponent = nullptr;

	UPROPERTY(VisibleDefaultsOnly)
	UParticleSystemComponent* PS_LeftControllerBeam = nullptr;

	UPROPERTY(VisibleDefaultsOnly)
	UParticleSystemComponent* PS_RightControllerBeam = nullptr;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~Functions~~ 

	UFUNCTION()
	void SetInitialActiveController();

	UFUNCTION()
	void UpdateMotionControllerPositions();

	UFUNCTION()
	void UpdateLaser();
	
};
