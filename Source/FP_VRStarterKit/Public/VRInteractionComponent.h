// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GenericVRPawn.h"
#include "VRInteractionComponent.generated.h"

class UWidgetComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FP_VRSTARTERKIT_API UVRInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UVRInteractionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadOnly)
	AGenericVRPawn* ParentPawn = nullptr;

	UPROPERTY(BlueprintReadOnly)
	bool RightControllerActive = false;

	UFUNCTION(BlueprintCallable, Category = "VR Interaction Component Functions")
	bool TraceForUI(USceneComponent* LaserSource, FVector& OutHitPoint, USceneComponent*& OutHitComponent);

	UFUNCTION(BlueprintCallable, Category = "VR Interaction Component Functions")
	void SetLeftLaserActive(bool newActive);

	UFUNCTION(BlueprintCallable, Category = "VR Interaction Component Functions")
	void SetRightLaserActive(bool newActive);

private:

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~Parent Components
	void GetParentPawnAndComponents();

	UParticleSystemComponent* PS_LeftLaserBeam = nullptr;
	UParticleSystemComponent* PS_RightLaserBeam = nullptr;
		
	
};