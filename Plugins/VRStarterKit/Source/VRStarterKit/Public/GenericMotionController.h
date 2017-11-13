// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MotionControllerComponent.h"
#include "GameFramework/Actor.h"
#include "GenericMotionController.generated.h"

UCLASS()
class VRSTARTERKIT_API AGenericMotionController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGenericMotionController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~Options~~
	UPROPERTY(EditAnywhere, Category = "Generic Motion Controller")
	//If true will prevent execution of begin play from the base class, allowing it to be fully implemented in blueprint
	bool bDoNotRunBaseBeginPlay = false;

	UPROPERTY(VisibleDefaultsOnly)
	USceneComponent* DefaultSceneRoot = nullptr;

	UPROPERTY(VisibleDefaultsOnly)
	UMotionControllerComponent* MotionController = nullptr;

	
	
};
