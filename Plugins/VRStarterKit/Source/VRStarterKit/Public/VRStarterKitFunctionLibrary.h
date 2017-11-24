// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "VRStarterKitFunctionLibrary.generated.h"

UENUM(BlueprintType)
enum class EVRLocomotionType : uint8
{
	LT_TeleportFixedRotation UMETA(DisplayName="Teleport Fixed Rotation"),
	LT_TeleportThumbStickRotation UMETA(DisplayName="Teleport Thumb Stick Rotation"),
	LT_Gamepad UMETA(DisplayName="Gamepad"),
	LT_FixedPosition UMETA(DisplayName="Fixed Position")
};

UCLASS()
class VRSTARTERKIT_API UVRStarterKitFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	
};
