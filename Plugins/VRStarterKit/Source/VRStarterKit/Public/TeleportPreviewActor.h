// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/CoreUObject/Public/UObject/Interface.h"
#include "TeleportPreviewActor.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTeleportPreviewActor : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class VRSTARTERKIT_API ITeleportPreviewActor
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Teleport Preview Actor")
	void SetPreviewPosition(const FVector Position);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Teleport Preview Actor")
	void SetPreviewRotation(const FRotator Rotation);
	
};
