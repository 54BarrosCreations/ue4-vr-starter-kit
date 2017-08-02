// Fill out your copyright notice in the Description page of Project Settings.

#include "VRCharacterInteractionComponent.h"
#include "GenericVRCharacter.h"
#include "VRWidgetComponent.h"


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
	GetParentCharacter();
}


// Called every frame
void UVRCharacterInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UVRCharacterInteractionComponent::TraceForUI(USceneComponent * LaserSource, FHitResult & OutHitResult)
{
	if (!ParentCharacter) return false; //return if parent object is null

	//Params
	FCollisionQueryParams TraceParameters;
	FHitResult HitResult;

	//Trace for ui hit
	if (GetWorld()->LineTraceSingleByChannel(
		OutHitResult,
		LaserSource->GetComponentLocation(),
		GetTraceEndPoint(LaserSource),
		ECollisionChannel::ECC_Visibility,
		TraceParameters)) {
		auto hitComponent = OutHitResult.GetComponent();
		if (hitComponent->IsA<UVRWidgetComponent>()) {
			if (SelectedWidget) SelectedWidget->DeselectWidget.Broadcast(ParentCharacter, this);
			SelectedWidget = Cast<UVRWidgetComponent>(hitComponent);
		}
	}
	
	
	return false;
}

FVector UVRCharacterInteractionComponent::GetTraceEndPoint(USceneComponent* LaserSource)
{
	if (ParentCharacter) return FVector(((LaserSource->GetForwardVector() + LaserSource->GetComponentRotation().Vector()) * ParentCharacter->LaserDrawDistance) + LaserSource->GetComponentLocation());
	else return FVector::ZeroVector;
}

FVector UVRCharacterInteractionComponent::GetLaserBeamEndPoint(USceneComponent * LaserSource, FHitResult HitResult)
{
	if (HitResult.bBlockingHit) return HitResult.ImpactPoint;
	else return GetTraceEndPoint(LaserSource);
}

void UVRCharacterInteractionComponent::GetParentCharacter()
{
	auto p = GetOwner();
	if (p->IsA<AGenericVRCharacter>()) ParentCharacter = Cast<AGenericVRCharacter>(p);
	else UE_LOG(LogTemp, Error, TEXT("InteractionComponent: Parent actor is not of type AGenericVRCharacter. Parent must inherit from this to function properly."));
}

