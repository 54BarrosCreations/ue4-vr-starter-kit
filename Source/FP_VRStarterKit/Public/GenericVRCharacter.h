// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
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

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~Inherited Components~~ 
	UPROPERTY(VisibleDefaultsOnly)
	//Camera Component Root, parent mesh object to this
	UCameraComponent* VRCameraOrigin = nullptr;
	
};
