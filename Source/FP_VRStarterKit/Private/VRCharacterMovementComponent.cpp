// Fill out your copyright notice in the Description page of Project Settings.

#include "VRCharacterMovementComponent.h"
#include "VRStarterKitErrorMessages.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStaticsTypes.h"
#include "Runtime/Engine/Classes/AI/Navigation/NavigationSystem.h"
#include "Engine.h"
#include "HeadMountedDisplay.h"

void UVRCharacterMovementComponent::InitializeMovementComponent(USceneComponent * TPBase, USceneComponent* TPDirection, USceneComponent * AEndPoint, UArrowComponent * LeftArcDirection, UArrowComponent * RightArcDirection)
{
	if (TPBase) TeleportBase = TPBase;
	if (TPDirection) TeleportDirection = TPDirection;
	if (AEndPoint) ArcEndPoint = AEndPoint;
	if (LeftArcDirection) LeftArrow = LeftArcDirection;
	if (RightArcDirection) RightArrow = RightArcDirection;

	if (!TeleportBase) LogError(GenerateErrorMessage(EVRErrorType::ET_OPTIONAL_COMPONENT_ERROR, "TeleportBase"));
	if (!TeleportDirection) LogError(GenerateErrorMessage(EVRErrorType::ET_OPTIONAL_COMPONENT_ERROR, "TeleportDirection"));
	if (!ArcEndPoint) LogError(GenerateErrorMessage(EVRErrorType::ET_OPTIONAL_COMPONENT_ERROR, "ArcEndPoint"));
	if (!LeftArrow) LogError(GenerateErrorMessage(EVRErrorType::ET_OPTIONAL_COMPONENT_ERROR, "LeftArrowComponent"));
	if (!RightArrow) LogError(GenerateErrorMessage(EVRErrorType::ET_OPTIONAL_COMPONENT_ERROR, "RightArrowComponent"));
}

void UVRCharacterMovementComponent::ActivateTeleporter(EControllerHand CallingHand)
{
	switch (CallingHand)
	{
	case EControllerHand::Left:
		bLeftTeleporterActive = true;
		bRightTeleporterActive = false;
		break;
	case EControllerHand::Right:
		bLeftTeleporterActive = false;
		bRightTeleporterActive = true;
		break;
	default:
		bLeftTeleporterActive = false;
		bRightTeleporterActive = false;
		break;
	}

	if (TeleportBase) TeleportBase->SetVisibility(true, true);
}

void UVRCharacterMovementComponent::DeactivateTeleporter()
{
	bLeftTeleporterActive = false;
	bRightTeleporterActive = false;

	if (TeleportBase) TeleportBase->SetVisibility(false, true);
	if (TeleportDirection) TeleportDirection->SetVisibility(false, true);
}

bool UVRCharacterMovementComponent::TraceTeleportDestination(FVector & outNavLocation, FVector & outTraceLocation, TArray<FVector>& outTracePoints, EControllerHand CallingHand, AActor* AdditionalActorToIgnore)
{
	FVector ArcStart;
	FVector LaunchVelocity;
	GetTeleportStartPointAndVelocity(CallingHand, ArcStart, LaunchVelocity);
	FPredictProjectilePathParams ProjectileParams;
	TArray<TEnumAsByte<EObjectTypeQuery>> ProjectileTypes;
	ProjectileTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);
	ProjectileParams.StartLocation = ArcStart;
	ProjectileParams.LaunchVelocity = LaunchVelocity;
	ProjectileParams.bTraceWithCollision = true;
	ProjectileParams.ProjectileRadius = 0.f;
	ProjectileParams.ActorsToIgnore.Add(GetOwner());
	if (AdditionalActorToIgnore) ProjectileParams.ActorsToIgnore.Add(AdditionalActorToIgnore);
	ProjectileParams.ObjectTypes = ProjectileTypes;
	FPredictProjectilePathResult Result;
	if (UGameplayStatics::PredictProjectilePath(this, ProjectileParams, Result)) {
		auto Nav = GetWorld()->GetNavigationSystem();
		if (!Nav) return false;
		FVector NavExtent = FVector(500.f, 500.f, 500.f);
		FNavLocation OutExtent;
		Nav->ProjectPointToNavigation(Result.HitResult.ImpactPoint, OutExtent, NavExtent);
		if (!Result.HitResult.Location.Equals(OutExtent, 0.0100)) {
			outNavLocation = OutExtent;
			outTraceLocation = Result.HitResult.ImpactPoint;
			for (FPredictProjectilePathPointData d : Result.PathData) outTracePoints.Add(d.Location);
			return true;

		}
	}

	return false;
}

void UVRCharacterMovementComponent::SetTeleportRotation(FVector2D AxisValues)
{
	if (!TeleportDirection) return;
	FRotator CharRot = FRotator(0, GetOwner()->GetActorRotation().Yaw, 0);
	if (VRMovementType == EVRMovementType::MT_TPThumbstickRotation) {
		FVector normalized = FVector(AxisValues.X, AxisValues.Y, 0);
		normalized.Normalize();
		FVector normalizedRotated = CharRot.RotateVector(normalized);
		FRotator FinalRot = normalizedRotated.ToOrientationRotator();

		if (FGenericPlatformMath::Abs(AxisValues.X) + FGenericPlatformMath::Abs(AxisValues.Y) >= 0.3) {
			TeleportRotation = FinalRot;
			if (TeleportDirection) TeleportDirection->SetVisibility(true, true);
		}else {
			TeleportRotation = CharRot;
			if (TeleportDirection) TeleportDirection->SetVisibility(false, true);
		}
	}else if (VRMovementType == EVRMovementType::MT_TPFixedRotation) {
		TeleportRotation = CharRot;
	}
	
}

void UVRCharacterMovementComponent::UpdateArcEndPoint(bool bValidLocationFound, FVector newLocation)
{
	if (!ArcEndPoint) return;
	if (bValidLocationFound) {
		ArcEndPoint->SetWorldLocation(newLocation);
		//if (!GEngine->HMDDevice.IsValid()) return;
		FVector pos = FVector::ZeroVector;
		FQuat rot = FQuat::Identity;
		//GEngine->HMDDevice->GetCurrentOrientationAndPosition(rot, pos);
		FRotator HmdRot = rot.Rotator();
		FRotator CombinedRotator = FRotator(0, TeleportRotation.Yaw + HmdRot.Yaw, 0);
		TeleportDirection->SetWorldRotation(CombinedRotator);
	}
}

bool UVRCharacterMovementComponent::GetTeleportDestination(FVector & OutLocation, FRotator & OutRotation)
{
	//if (!GEngine->HMDDevice.IsValid()) return false;
	FVector hmdPos;
	FQuat hmdQuat;
//	GEngine->HMDDevice->GetCurrentOrientationAndPosition(hmdQuat, hmdPos);
	if (VRMovementType == EVRMovementType::MT_TPThumbstickRotation) {
		FVector HmdUnrotated = TeleportRotation.RotateVector(FVector(hmdPos.X, hmdPos.Y, 0));
		FVector HmdFinal = TeleportBase->GetComponentLocation() - HmdUnrotated;
		OutLocation = FVector(HmdFinal.X, HmdFinal.Y, 0);
		OutRotation = TeleportRotation;
	}
	else {
		FRotator pawnRot = UGameplayStatics::GetPlayerPawn(this, 0)->GetActorRotation();
		FVector HmdFinal = TeleportBase->GetComponentLocation() - pawnRot.RotateVector(hmdPos);
		OutLocation = FVector(HmdFinal.X, HmdFinal.Y, 0);
		OutRotation = pawnRot;
	}
	return true;
}

bool UVRCharacterMovementComponent::GetTeleportStartPointAndVelocity(EControllerHand Hand, FVector & outArcStart, FVector & outLaunchVelocity)
{
	switch (Hand)
	{
	case EControllerHand::Left:
		outArcStart = LeftArrow->GetComponentLocation();
		outLaunchVelocity = LeftArrow->GetForwardVector() * TeleportLaunchVelocity;
		break;
	case EControllerHand::Right:
		outArcStart = RightArrow->GetComponentLocation();
		outLaunchVelocity = RightArrow->GetForwardVector() * TeleportLaunchVelocity;
		break;
	default:
		outArcStart = FVector::ZeroVector;
		outLaunchVelocity = FVector::ZeroVector;
		return false;
		break;
	}

	return true;
}



