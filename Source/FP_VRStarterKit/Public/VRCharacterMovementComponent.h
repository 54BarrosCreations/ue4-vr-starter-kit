// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "VRCharacterMovementComponent.generated.h"

UENUM(BlueprintType)
enum class EVRMovementType : uint8 {
	MT_TPFixedRotation UMETA(DisplayName="Teleport Fixed Rotatation"),
	MT_TPThumbstickRotation UMETA(DisplayName = "Teleport Thumbstick Rotation"),
	MT_Gamepad UMETA(DisplayName = "Gamepad")
};

UCLASS()
class FP_VRSTARTERKIT_API UVRCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "VR Character Movement Component")
	EVRMovementType VRMovementType = EVRMovementType::MT_TPFixedRotation;

	UFUNCTION(BlueprintCallable, Category = "VR MovementComponent")
	void ActivateTeleporter();

	UPROPERTY(BlueprintReadOnly)
	bool bTeleporterActive = false;
	
	
};
