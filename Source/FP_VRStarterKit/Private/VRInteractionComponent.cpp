// Fill out your copyright notice in the Description page of Project Settings.

#include "VRInteractionComponent.h"
#include "Runtime/UMG/Public/Components/WidgetComponent.h"
#include "VRWidgetComponent.h"


// Sets default values for this component's properties
UVRInteractionComponent::UVRInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UVRInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UVRInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UVRInteractionComponent::TraceForUI(USceneComponent* LaserSource, FVector& OutHitPoint, USceneComponent*& OutHitComponent)
{
	//Setup Query Parameters
	FCollisionQueryParams TraceParameters(FName(TEXT(" ")), false, ParentPawn);
	FHitResult HitResult;

	//Trace for ui hit
	GetWorld()->LineTraceSingleByChannel(
		HitResult,
		LaserSource->GetComponentLocation(),
		FVector(((LaserSource->GetForwardVector() + LaserSource->GetComponentRotation().Vector()) * ParentPawn->LaserDrawDistance) + LaserSource->GetComponentLocation()),
		ECollisionChannel::ECC_Visibility,
		TraceParameters
	);

	//Check for a hit, and pass relevant information to our out parameters
	if (HitResult.bBlockingHit) {
		OutHitPoint = HitResult.ImpactPoint;
		OutHitComponent = HitResult.GetComponent();

		if (!OutHitComponent) return false; //<--Return if there is a hit, but no hit component, because something went horribly wrong.

		if (OutHitComponent->IsA<UVRWidgetComponent>() || OutHitComponent->IsA<UWidgetComponent>()) return true;
		else return false;
	}

	//Set out params to default values if there is no hit
	OutHitPoint = FVector::ZeroVector;
	OutHitComponent = nullptr;
	return false;
}

