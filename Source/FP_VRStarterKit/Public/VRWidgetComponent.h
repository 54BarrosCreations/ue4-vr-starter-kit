// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "VRWidgetComponent.generated.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~Forward Declarations
class AGenericVRCharacter;
class UVRCharacterInteractionComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWidgetEvent, AGenericVRCharacter*, CallingPawn, UVRCharacterInteractionComponent*, InteractionComponent);

UCLASS(ClassGroup = (Interaction), meta = (BlueprintSpawnableComponent))
class FP_VRSTARTERKIT_API UVRWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:

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
