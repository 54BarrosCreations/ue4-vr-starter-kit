// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "PickupObject.h"
#include "VRCharacterInteractionComponent.h"
#include "VRCharacterMovementComponent.h"
#include "MotionControllerComponent.h"
#include "SteamVRFunctionLibrary.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "HeadMountedDisplay.h"
#include "Runtime/HeadMountedDisplay/Public/HeadMountedDisplayTypes.h"
#include "PickupObject.h"
#include "GameFramework/Character.h"
#include "GenericVRCharacter.generated.h"

UCLASS()
class FP_VRSTARTERKIT_API AGenericVRCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGenericVRCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	EHMDDeviceType::Type HMD;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~VR Character Options~~
	UPROPERTY(EditAnywhere, Category = "Generic VR Character|Motion Controllers")
	bool bUseLaserInteraction = false;

	UPROPERTY(EditAnywhere, Category = "Generic VR Character|Motion Controllers", meta = (EditCondition = "bUseLaserInteraction"))
	float LaserDrawDistance = 5000.f;

	UPROPERTY(EditAnywhere, Category = "Generic VR Character|Motion Controllers")
	bool bAllowGripping = false;

	UPROPERTY(EditAnywhere, Category = "Generic VR Character|Motion Controllers")
	bool bAllowTeleportation = false;

	UPROPERTY(EditAnywhere, Category = "Generic VR Character")
	//If false, will keep character mesh from rotating with hmd. Useful for meshes that only need positional tracking.
	bool bAllowCharacterMeshRotation = true;

	UPROPERTY(EditAnywhere, Category = "Generic VR Character")
	//If false, will keep optional mesh from rotating with hmd. Useful for meshes that only need positional tracking.
	bool bAllowOptionalMeshRotation = false;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~Inherited Components~~ 
	UPROPERTY(VisibleDefaultsOnly)
	//Camera Component Root, parent mesh object to this
	USceneComponent* VRSceneRoot = nullptr;

	UPROPERTY(VisibleDefaultsOnly)
	//Camera Component Root, parent mesh object to this
	USceneComponent* VROrigin = nullptr;

	UPROPERTY(VisibleDefaultsOnly)
	//Camera Component Root, parent mesh object to this
	UCameraComponent* DefaultCamera = nullptr;

	UPROPERTY(VisibleDefaultsOnly)
	USceneComponent* DefaultCharacterMeshRoot = nullptr;

	UPROPERTY(VisibleDefaultsOnly)
	UMotionControllerComponent* LeftMotionController = nullptr;

	UPROPERTY(VisibleDefaultsOnly)
	UMotionControllerComponent* RightMotionController = nullptr;

	UPROPERTY(VisibleDefaultsOnly)
	UParticleSystemComponent* PS_LeftControllerBeam = nullptr;

	UPROPERTY(VisibleDefaultsOnly)
	UParticleSystemComponent* PS_RightControllerBeam = nullptr;

	UPROPERTY(VisibleDefaultsOnly)
	UVRCharacterInteractionComponent* InteractionComponent = nullptr;

	//Left Controller Replication
	UPROPERTY(VisibleDefaultsOnly)
	USceneComponent* ReplicatedLeftControllerRoot = nullptr;

	UPROPERTY(VisibleDefaultsOnly)
	UStaticMeshComponent* SM_ReplicatedLeftController = nullptr;

	//Right Controller Replication
	UPROPERTY(VisibleDefaultsOnly)
	USceneComponent* ReplicatedRightControllerRoot = nullptr;

	UPROPERTY(VisibleDefaultsOnly)
	UStaticMeshComponent* SM_ReplicatedRightController = nullptr;

	UPROPERTY(BlueprintReadOnly)
	UVRCharacterMovementComponent * VRMovementComponent = nullptr;

	UPROPERTY(BlueprintReadWrite)
	EControllerHand ActiveTeleportHand = EControllerHand::Special_9;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~Optional Components~~ 

	USphereComponent* GrabSphere_L = nullptr;
	USphereComponent* GrabSphere_R = nullptr;
	USceneComponent* OptionalCharacterMesh = nullptr;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~Functions~~ 

	UFUNCTION()
	void SetInitialActiveController();

	UFUNCTION()
	void UpdateMotionControllerPositions();

	UFUNCTION()
	void UpdateLaser();

	UFUNCTION()
	void RenderTeleportPreview();

	UFUNCTION()
	void ExecuteTeleport();

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~Replication~~
	UFUNCTION(Server, Unreliable, WithValidation)
	void ServerUpdateControllerPos(FTransform LeftControllerTransform, FTransform RightControllerTransform);
	void ServerUpdateControllerPos_Implementation(FTransform LeftControllerTransform, FTransform RightControllerTransform);
	bool ServerUpdateControllerPos_Validate(FTransform LeftControllerTransform, FTransform RightControllerTransform);

	UFUNCTION(NetMulticast, Unreliable, WithValidation)
	void MulticastUpdateControllerPos(FTransform LeftControllerTransform, FTransform RightControllerTransform);
	void MulticastUpdateControllerPos_Implementation(FTransform LeftControllerTransform, FTransform RightControllerTransform);
	bool MulticastUpdateControllerPos_Validate(FTransform LeftControllerTransform, FTransform RightControllerTransform);

	UFUNCTION(Server, Unreliable, WithValidation)
	void ServerUpdateCharacterMeshPos(FVector HMDPos, FRotator HMDRot);
	void ServerUpdateCharacterMeshPos_Implementation(FVector HMDPos, FRotator HMDRot);
	bool ServerUpdateCharacterMeshPos_Validate(FVector HMDPos, FRotator HMDRot);

	UFUNCTION(NetMulticast, Unreliable, WithValidation)
	void MulticastUpdateCharacterMeshPos(FVector HMDPos, FRotator HMDRot);
	void MulticastUpdateCharacterMeshPos_Implementation(FVector HMDPos, FRotator HMDRot);
	bool MulticastUpdateCharacterMeshPos_Validate(FVector HMDPos, FRotator HMDRot);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "VR Character Events")
	void SetReplicatedComponentsVisible(bool newVisibility);


	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~Controller Events~~

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "VR Character Controller Events")
	void LeftMotionControllerTriggerDown(UVRCharacterInteractionComponent* VRInteractionComponent);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "VR Character Controller Events")
	void LeftMotionControllerTriggerUp(UVRCharacterInteractionComponent* VRInteractionComponent);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "VR Character Controller Events")
	void RightMotionControllerTriggerDown(UVRCharacterInteractionComponent* VRInteractionComponent);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "VR Character Controller Events")
	void RightMotionControllerTriggerUp(UVRCharacterInteractionComponent* VRInteractionComponent);

private:

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~Input~~
	void LeftTriggerDown();
	void LeftTriggerUp();
	void RightTriggerDown();
	void RightTriggerUp();
	void LeftGripDown();
	void LeftGripUp();
	void RightGripDown();
	void RightGripUp();
	void LeftThumbStickDown();
	void LeftThumbStickUp();
	void RightThumbStickDown();
	void RightThumbStickUp();

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~Gripping~~
	AActor* GetClosestValidActor(TArray<AActor*> InOverlappingActors);
	AActor* RightGrippedActor = nullptr;
	AActor* LeftGrippedActor = nullptr;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~VR Setup~~

	void InitializeHMD();
	void GetOptionalComponents();

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~Utilities~~
	FTransform GetReplicatedTransform(FTransform ReplicatedTransform, bool bAllowRotation);
	bool bIsTeleporting = false;
	bool bLastFrameValidTeleportDestination = false;
	
};
