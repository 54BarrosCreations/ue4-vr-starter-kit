// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "VRWidgetComponent.generated.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~Forward Declarations
class AGenericVRPawn;
class UVRInteractionComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWidgetEvent, AGenericVRPawn*, CallingPawn, UVRInteractionComponent*, InteractionComponent);

UCLASS(ClassGroup = (Interaction), meta = (BlueprintSpawnableComponent))
class FP_VRSTARTERKIT_API UVRWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintAssignable, Category = Events)
	FWidgetEvent MotionControllerTriggerDown;

	UPROPERTY(BlueprintAssignable, Category = Events)
	FWidgetEvent MotionControllerTriggerUp;

	UPROPERTY(BlueprintAssignable, Category = Events)
	FWidgetEvent HighlightWidget;

	UPROPERTY(BlueprintAssignable, Category = Events)
	FWidgetEvent SelectWidget;

	UPROPERTY(BlueprintAssignable, Category = Events)
	FWidgetEvent DeselectWidget;


	
	
};
