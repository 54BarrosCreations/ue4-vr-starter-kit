// Fill out your copyright notice in the Description page of Project Settings.

#include "VRInteractionComponent.h"
#include "Runtime/UMG/Public/Components/WidgetComponent.h"
#include "GenericVRPawn.h"
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
	GetParentPawnAndComponents();
	
}


// Called every frame
void UVRInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UVRInteractionComponent::TraceForUI(USceneComponent * LaserSource)
{
	//Setup Query Parameters
	FCollisionQueryParams TraceParameters(FName(TEXT(" ")), false, ParentPawn);
	FHitResult HitResult;

	//Trace for ui hit
	auto W = GetWorld();
	if (!W || !ParentPawn) return false;

	GetWorld()->LineTraceSingleByChannel(
		HitResult,
		LaserSource->GetComponentLocation(),
		FVector(((LaserSource->GetForwardVector() + LaserSource->GetComponentRotation().Vector()) * ParentPawn->LaserDrawDistance) + LaserSource->GetComponentLocation()),
		ECollisionChannel::ECC_Visibility,
		TraceParameters
	);

	//Check for a hit, return true if a valid ui hit
	if (HitResult.bBlockingHit) {
		auto lHitComponent = HitResult.GetComponent();
		if (!lHitComponent) return false; //<--Return if there is a hit, but no hit component, because something went horribly wrong.
		if (lHitComponent->IsA<UVRWidgetComponent>()) {
			HitComponent = lHitComponent;
			if (SelectedWidget) SelectedWidget->DeselectWidget.Broadcast(ParentPawn, this);
			SelectedWidget = Cast<UVRWidgetComponent>(HitComponent);
			SelectedWidget->HighlightWidget.Broadcast(ParentPawn, this);
			return true;
		}

		if (SelectedWidget) SelectedWidget->DeselectWidget.Broadcast(ParentPawn, this);
		SelectedWidget = nullptr;
		HitComponent = lHitComponent;
		return false;
	}

	if (SelectedWidget) SelectedWidget->DeselectWidget.Broadcast(ParentPawn, this);
	SelectedWidget = nullptr;
	HitComponent = nullptr;
	return false;
}

bool UVRInteractionComponent::TraceForUI(USceneComponent* LaserSource, FHitResult &OutHitResult)
{
	//Setup Query Parameters
	FCollisionQueryParams TraceParameters(FName(TEXT(" ")), false, ParentPawn);
	FHitResult HitResult;

	//Trace for ui hit
	auto W = GetWorld();
	if (!W || !ParentPawn) return false;

	GetWorld()->LineTraceSingleByChannel(
		HitResult,
		LaserSource->GetComponentLocation(),
		FVector(((LaserSource->GetForwardVector() + LaserSource->GetComponentRotation().Vector()) * ParentPawn->LaserDrawDistance) + LaserSource->GetComponentLocation()),
		ECollisionChannel::ECC_Visibility,
		TraceParameters
	);

	//Out Parameter
	OutHitResult = HitResult;

	//Check for a hit, return true if a valid ui hit
	if (HitResult.bBlockingHit) {
		auto lHitComponent = HitResult.GetComponent();
		if (!lHitComponent) return false; //<--Return if there is a hit, but no hit component, because something went horribly wrong.
		if (lHitComponent->IsA<UVRWidgetComponent>()) {
			HitComponent = lHitComponent;
			if (SelectedWidget) SelectedWidget->DeselectWidget.Broadcast(ParentPawn, this);
			SelectedWidget = Cast<UVRWidgetComponent>(HitComponent);
			SelectedWidget->HighlightWidget.Broadcast(ParentPawn, this);
			return true;
		}
		
		if (SelectedWidget) SelectedWidget->DeselectWidget.Broadcast(ParentPawn, this);
		SelectedWidget = nullptr;
		HitComponent = lHitComponent;
		return false;
	}

	if (SelectedWidget) SelectedWidget->DeselectWidget.Broadcast(ParentPawn, this);
	SelectedWidget = nullptr;
	HitComponent = nullptr;
	return false;
}

void UVRInteractionComponent::SetRightLaserActive(bool newActive)
{
	RightControllerActive = newActive;
}

FVector UVRInteractionComponent::GetBeamEndPoint(USceneComponent * LaserSource, FHitResult HitResult)
{
	if (!ParentPawn) return FVector::ZeroVector;
	if (HitResult.bBlockingHit) {
		return HitResult.ImpactPoint;
	}else {
		return FVector(((LaserSource->GetForwardVector() + LaserSource->GetComponentRotation().Vector()) * ParentPawn->LaserDrawDistance) + LaserSource->GetComponentLocation());
	}
}

void UVRInteractionComponent::GetParentPawnAndComponents()
{
	auto p = GetOwner();
	if (p->IsA<AGenericVRPawn>()) ParentPawn = Cast<AGenericVRPawn>(p);
	else return; //<--Return if pawn is of incorrect type

	if (ParentPawn->PS_LeftControllerBeam) PS_LeftLaserBeam = ParentPawn->PS_LeftControllerBeam;
	else UE_LOG(LogTemp, Error, TEXT("%s: Interaction Component: No Particle system assigned to PS_LeftControllerBeam."), *ParentPawn->GetName());

	if (ParentPawn->PS_LeftControllerBeam) PS_RightLaserBeam = ParentPawn->PS_RightControllerBeam;
	else UE_LOG(LogTemp, Error, TEXT("%s: Interaction Component: No Particle system assigned to PS_RightControllerBeam."), *ParentPawn->GetName());
	
}

