// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Components/SplineComponent.h"
#include "Runtime/Engine/Classes/Components/SplineMeshComponent.h"
#include "Runtime/Engine/Classes/Components/ArrowComponent.h"
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

	UPROPERTY(EditAnywhere, Category = "VR Character Movement Component")
	float TeleportLaunchVelocity = 5000.f;

	UPROPERTY(BlueprintReadOnly)
	USceneComponent* TeleportBase = nullptr;

	UPROPERTY(BlueprintReadOnly)
	USceneComponent* ArcEndPoint = nullptr;

	UPROPERTY(BlueprintReadOnly)
	UArrowComponent* LeftArrow = nullptr;

	UPROPERTY(BlueprintReadOnly)
	UArrowComponent* RightArrow = nullptr;

	UFUNCTION(BlueprintCallable, Category = "VR Movement Component")
	void InitializeMovementComponent(USceneComponent* TPBase, USceneComponent* AEndPoint, UArrowComponent* LeftArcDirection, UArrowComponent* RightArcDirection);

	UFUNCTION(BlueprintCallable, Category = "VR Movement Component")
	void ActivateTeleporter(EControllerHand CallingHand);

	UFUNCTION(BlueprintCallable, Category = "VR Movement Component")
	void DeactivateTeleporter();

	UFUNCTION(BlueprintCallable, Category = "VR Movement Component")
	bool TraceTeleportDestination(FVector &outNavLocation, FVector &outTraceLocation, TArray<FVector> &outTracePoints, EControllerHand CallingHand);

	UFUNCTION(BlueprintCallable, Category = "VR Movement Component")
	void UpdateArcEndPoint(bool bValidLocationFound, FVector newLocation);

	UFUNCTION(BlueprintPure, Category = "VR Movement Component")
	bool GetTeleportDestination(FVector& OutLocation, FRotator & OutRotation);

	UFUNCTION(BlueprintPure, Category = "VR MovementComponent")
	bool GetTeleportLaunchVelocity(EControllerHand Hand, FVector &outArcStart, FVector &outLaunchVelocity);
	 
	UPROPERTY(BlueprintReadWrite)
	bool bRightTeleporterActive = false;

	UPROPERTY(BlueprintReadWrite)
	bool bLeftTeleporterActive = false;

	UPROPERTY(BlueprintReadWrite)
	FRotator TeleportRotation;
	
	
};
