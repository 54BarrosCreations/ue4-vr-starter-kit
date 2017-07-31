// Fill out your copyright notice in the Description page of Project Settings.

#include "VRCharacterInteractionComponent.h"


// Sets default values for this component's properties
UVRCharacterInteractionComponent::UVRCharacterInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UVRCharacterInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UVRCharacterInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UVRCharacterInteractionComponent::TraceForUI(USceneComponent * TraceStart, FVector TraceEnd, FHitResult & OutHitResult)
{
	FCollisionQueryParams Params;
	return false;
}

