// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VRInteractionComponent.generated.h"

class UWidgetComponent;
class AGenericVRPawn;

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

	bool ControllerFirstTimeActive = true;

	UPROPERTY(BlueprintReadOnly)
	bool RightControllerActive = false;

	bool TraceForUI(USceneComponent* LaserSource);

	bool TraceForUI(USceneComponent* LaserSource, FHitResult &OutHitResult);

	UFUNCTION(BlueprintCallable, Category = "VR Interaction Component Functions")
	void SetRightLaserActive(bool newActive);

	UFUNCTION(BlueprintCallable, Category = "VR Interaction Component Functions")
	FVector GetBeamEndPoint(USceneComponent* LaserSource, FHitResult HitResult);

	USceneComponent* HitComponent = nullptr;

private:

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~Parent Components
	void GetParentPawnAndComponents();

	UParticleSystemComponent* PS_LeftLaserBeam = nullptr;
	UParticleSystemComponent* PS_RightLaserBeam = nullptr;
		
	
};
