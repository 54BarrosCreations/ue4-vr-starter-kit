// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupObject.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPickupObject : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FP_VRSTARTERKIT_API IPickupObject
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Pickup Interface")
	bool GrabObject(AActor* ActorToGrab);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Pickup Interface")
	bool ReleaseObject();
	
};
