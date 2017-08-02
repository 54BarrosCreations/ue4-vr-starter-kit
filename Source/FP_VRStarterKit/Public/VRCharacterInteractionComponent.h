// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VRCharacterInteractionComponent.generated.h"

class UWidgetComponent;
class UVRWidgetComponent;
class AGenericVRCharacter;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FP_VRSTARTERKIT_API UVRCharacterInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UVRCharacterInteractionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadOnly)
	AGenericVRCharacter* ParentCharacter = nullptr;

	bool bControllerFirstTimeActive = true;

	UPROPERTY(BlueprintReadOnly)
	bool bRightControllerActive = false;
	
	UFUNCTION(BlueprintCallable, Category = "Interaction Component Functions")
	bool TraceForUI(USceneComponent* TraceStart, FHitResult &OutHitResult);

	UFUNCTION(BlueprintCallable, Category = "Interaction Component Functions")
	FVector GetTraceEndPoint(USceneComponent* LaserSource);

	UFUNCTION(BlueprintCallable, Category = "Interaction Component Functions")
	FVector GetLaserBeamEndPoint(USceneComponent* LaserSource, FHitResult HitResult);

	UPROPERTY(BlueprintReadOnly)
	UVRWidgetComponent* SelectedWidget = nullptr;

private:
	void GetParentCharacter();
	
};
