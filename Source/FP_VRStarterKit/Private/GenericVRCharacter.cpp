// Fill out your copyright notice in the Description page of Project Settings.

#include "GenericVRCharacter.h"
#include "VRStarterKitErrorMessages.h"
#include "VRWidgetComponent.h"


// Sets default values
AGenericVRCharacter::AGenericVRCharacter(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer.DoNotCreateDefaultSubobject(ACharacter::MeshComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	VRSceneRoot = CreateDefaultSubobject<USceneComponent>(FName("VR Scene Root"));
	RootComponent = VRSceneRoot;
	VROrigin = CreateDefaultSubobject<USceneComponent>(TEXT("VR Origin"));
	VROrigin->SetupAttachment(VRSceneRoot);
	DefaultCamera = CreateDefaultSubobject<UCameraComponent>(FName("VR Camera Origin"));
	DefaultCamera->SetupAttachment(VROrigin);
	GetCapsuleComponent()->SetupAttachment(DefaultCamera);

	// Character Mesh Root
	DefaultCharacterMeshRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Character Mesh Root"));
	DefaultCharacterMeshRoot->SetupAttachment(VROrigin);

	//Left Motion Controller
	LeftMotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Left Controller"));
	LeftMotionController->SetupAttachment(VROrigin);
	LeftMotionController->SetIsReplicated(false);
	LeftMotionController->Hand = EControllerHand::Left;

	//Left Controller Replicated Components
	ReplicatedLeftControllerRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Replicated Left Controller Root"));
	ReplicatedLeftControllerRoot->SetupAttachment(VROrigin);
	SM_ReplicatedLeftController = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Replicated Left Controller Mesh"));
	SM_ReplicatedLeftController->SetupAttachment(ReplicatedLeftControllerRoot);
	PS_LeftControllerBeam = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Left Controller Beam"));
	PS_LeftControllerBeam->SetupAttachment(ReplicatedLeftControllerRoot);

	//Right Motion Controller
	RightMotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Right Controller"));
	RightMotionController->SetupAttachment(VROrigin);
	RightMotionController->SetIsReplicated(false);
	RightMotionController->Hand = EControllerHand::Right;

	//Right Controller Replicated Components
	ReplicatedRightControllerRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Replicated Right Controller Root"));
	ReplicatedRightControllerRoot->SetupAttachment(VROrigin);
	SM_ReplicatedRightController = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Replicated Right Controller Mesh"));
	SM_ReplicatedRightController->SetupAttachment(ReplicatedRightControllerRoot);
	PS_RightControllerBeam = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Right Controller Beam"));
	PS_RightControllerBeam->SetupAttachment(ReplicatedRightControllerRoot);

	//Interaction Component
	InteractionComponent = CreateDefaultSubobject<UVRCharacterInteractionComponent>(TEXT("Interaction Component"));
}

// Called when the game starts or when spawned
void AGenericVRCharacter::BeginPlay()
{
	Super::BeginPlay();
	InitializeHMD();
	GetOptionalComponents();
}

// Called every frame
void AGenericVRCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bUseLaserInteraction)	UpdateLaser();
	if (IsLocallyControlled()) {
		ServerUpdateControllerPos(LeftMotionController->GetComponentTransform(), RightMotionController->GetComponentTransform());

		FVector Pos;
		FQuat Rot;
		GEngine->HMDDevice->GetCurrentOrientationAndPosition(Rot, Pos);
		ServerUpdateCharacterMeshPos(Pos, Rot.Rotator());
	}
}

// Called to bind functionality to input
void AGenericVRCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("LeftTrigger", EInputEvent::IE_Pressed, this, &AGenericVRCharacter::LeftTriggerDown);
	PlayerInputComponent->BindAction("LeftTrigger", EInputEvent::IE_Released, this, &AGenericVRCharacter::LeftTriggerUp);
	PlayerInputComponent->BindAction("RightTrigger", EInputEvent::IE_Pressed, this, &AGenericVRCharacter::RightTriggerDown);
	PlayerInputComponent->BindAction("RightTrigger", EInputEvent::IE_Released, this, &AGenericVRCharacter::RightTriggerUp);
	PlayerInputComponent->BindAction("LeftGrip", EInputEvent::IE_Pressed, this, &AGenericVRCharacter::LeftGripDown);
	PlayerInputComponent->BindAction("LeftGrip", EInputEvent::IE_Released, this, &AGenericVRCharacter::LeftGripUp);
	PlayerInputComponent->BindAction("RightGrip", EInputEvent::IE_Pressed, this, &AGenericVRCharacter::RightGripDown);
	PlayerInputComponent->BindAction("RightGrip", EInputEvent::IE_Released, this, &AGenericVRCharacter::RightGripUp);
}

void AGenericVRCharacter::SetInitialActiveController()
{
	if (!InteractionComponent) return;
	FHitResult HitResult;
	if (InteractionComponent->TraceForUI(LeftMotionController, HitResult)) {
		InteractionComponent->bRightControllerActive = false;
		InteractionComponent->bControllerFirstTimeActive = false;
	}
	else if (InteractionComponent->TraceForUI(RightMotionController, HitResult)) {
		InteractionComponent->bRightControllerActive = true;
		InteractionComponent->bControllerFirstTimeActive = false;
	}
}

void AGenericVRCharacter::UpdateMotionControllerPositions()
{
	//Left Controller
	FVector LPos;
	FRotator LRot;
	USteamVRFunctionLibrary::GetHandPositionAndOrientation(0, EControllerHand::Left, LPos, LRot);
	LeftMotionController->SetRelativeTransform(FTransform(LRot, LPos, FVector(1, 1, 1)));

	//Right Controller
	FVector RPos;
	FRotator RRot;
	USteamVRFunctionLibrary::GetHandPositionAndOrientation(0, EControllerHand::Right, RPos, RRot);
	RightMotionController->SetRelativeTransform(FTransform(RRot, RPos, FVector(1, 1, 1)));
}

void AGenericVRCharacter::UpdateLaser()
{
	if (InteractionComponent->bControllerFirstTimeActive) SetInitialActiveController();
	else {
		FHitResult HitResult;
		if (InteractionComponent->bRightControllerActive) {
			if (InteractionComponent->TraceForUI(RightMotionController, HitResult)) {
				PS_RightControllerBeam->SetBeamSourcePoint(0, RightMotionController->GetComponentLocation(), 0);
				PS_RightControllerBeam->SetBeamEndPoint(0, InteractionComponent->GetLaserBeamEndPoint(RightMotionController, HitResult));
				if (PS_LeftControllerBeam->bVisible) PS_LeftControllerBeam->SetVisibility(false);
				if (!PS_RightControllerBeam->bVisible) PS_RightControllerBeam->SetVisibility(true);
			}
			else {
				if (PS_RightControllerBeam->bVisible) PS_RightControllerBeam->SetVisibility(false);
			}
		}
		else if (!InteractionComponent->bRightControllerActive) {
			if (InteractionComponent->TraceForUI(LeftMotionController, HitResult)) {
				PS_LeftControllerBeam->SetBeamSourcePoint(0, LeftMotionController->GetComponentLocation(), 0);
				PS_LeftControllerBeam->SetBeamEndPoint(0, InteractionComponent->GetLaserBeamEndPoint(LeftMotionController, HitResult));
				if (PS_RightControllerBeam->bVisible) PS_RightControllerBeam->SetVisibility(false);
				if (!PS_LeftControllerBeam->bVisible) PS_LeftControllerBeam->SetVisibility(true);
			}
			else {
				if (PS_LeftControllerBeam->bVisible) PS_LeftControllerBeam->SetVisibility(false);
			}
		}
	}
}

void AGenericVRCharacter::ServerUpdateControllerPos_Implementation(FTransform LeftControllerTransform, FTransform RightControllerTransform)
{
	MulticastUpdateControllerPos(LeftControllerTransform, RightControllerTransform);
}

bool AGenericVRCharacter::ServerUpdateControllerPos_Validate(FTransform LeftControllerTransform, FTransform RightControllerTransform)
{
	return true;
}

void AGenericVRCharacter::MulticastUpdateControllerPos_Implementation(FTransform LeftControllerTransform, FTransform RightControllerTransform)
{
	ReplicatedLeftControllerRoot->SetWorldTransform(LeftControllerTransform);
	ReplicatedRightControllerRoot->SetWorldTransform(RightControllerTransform);
}

bool AGenericVRCharacter::MulticastUpdateControllerPos_Validate(FTransform LeftControllerTransform, FTransform RightControllerTransform)
{
	return true;
}

void AGenericVRCharacter::ServerUpdateCharacterMeshPos_Implementation(FVector HMDPos, FRotator HMDRot)
{
	MulticastUpdateCharacterMeshPos(HMDPos, HMDRot);
}

bool AGenericVRCharacter::ServerUpdateCharacterMeshPos_Validate(FVector HMDPos, FRotator HMDRot)
{
	return true;
}

void AGenericVRCharacter::MulticastUpdateCharacterMeshPos_Implementation(FVector HMDPos, FRotator HMDRot)
{
	FRotator DefaultMeshRotation;
	FRotator OptionalMeshRotation;

	if (bAllowCharacterMeshRotation) DefaultMeshRotation = HMDRot;
	else DefaultMeshRotation = FRotator::ZeroRotator;

	if (bAllowOptionalMeshRotation) OptionalMeshRotation = HMDRot;
	else OptionalMeshRotation = FRotator::ZeroRotator;

	DefaultCharacterMeshRoot->SetRelativeLocationAndRotation(HMDPos, DefaultMeshRotation);
	if (OptionalCharacterMesh) OptionalCharacterMesh->SetRelativeLocationAndRotation(HMDPos, OptionalMeshRotation);
	
}

bool AGenericVRCharacter::MulticastUpdateCharacterMeshPos_Validate(FVector HMDPos, FRotator HMDRot)
{
	return true;
}


void AGenericVRCharacter::LeftTriggerDown()
{
	if (!InteractionComponent) return;

	//Check for UI Interaction
	if (InteractionComponent->bRightControllerActive) {
		FHitResult HitResult;
		if (InteractionComponent->TraceForUI(LeftMotionController, HitResult)) InteractionComponent->bRightControllerActive = false;
	} else {
		if (InteractionComponent->TraceHitResultComponent) {
			if (InteractionComponent->TraceHitResultComponent->IsA<UVRWidgetComponent>()) {
				Cast<UVRWidgetComponent>(InteractionComponent->TraceHitResultComponent)->MotionControllerTriggerDown.Broadcast(this, InteractionComponent);
			}
		}
	}

	//Call blueprint event
	LeftMotionControllerTriggerDown(InteractionComponent);
}

void AGenericVRCharacter::LeftTriggerUp()
{
	LeftMotionControllerTriggerUp(InteractionComponent);
}

void AGenericVRCharacter::RightTriggerDown()
{
	//Check for Ui Interaction
	if (bUseLaserInteraction && InteractionComponent) {
		if (!InteractionComponent->bRightControllerActive) {
			FHitResult HitResult;
			if (InteractionComponent->TraceForUI(RightMotionController, HitResult)) InteractionComponent->bRightControllerActive = true;
		} else {
			if (InteractionComponent->TraceHitResultComponent) {
				if (InteractionComponent->TraceHitResultComponent->IsA<UVRWidgetComponent>()) {
					Cast<UVRWidgetComponent>(InteractionComponent->TraceHitResultComponent)->MotionControllerTriggerDown.Broadcast(this, InteractionComponent);
				}
			}
		}
	}
	
	

	//Call blueprint event
	RightMotionControllerTriggerDown(InteractionComponent);
}

void AGenericVRCharacter::RightTriggerUp()
{
	RightMotionControllerTriggerUp(InteractionComponent);
}

void AGenericVRCharacter::LeftGripDown()
{
	//Check for Grabbable actors
	if (bAllowGripping && GrabSphere_L) {
		TArray<AActor*> OverlappingActors;
		GrabSphere_L->GetOverlappingActors(OverlappingActors);
		AActor* NearestActor = GetClosestValidActor(OverlappingActors);
		if (NearestActor) {
			if (IPickupObject::Execute_GrabObject(NearestActor, ReplicatedLeftControllerRoot, false)) LeftGrippedActor = NearestActor;
		}
		else LogWarning("No Actors with interface found.");
	}
	else if (bAllowGripping && !GrabSphere_L) {
		LogError(GenerateErrorMessage(EVRErrorType::ET_FUNCTION_UNAVAILABLE_COMPONENT_ERROR, "Grab", "Left grab sphere missing."));
	}
}

void AGenericVRCharacter::LeftGripUp()
{
	if (LeftGrippedActor) IPickupObject::Execute_ReleaseObject(LeftGrippedActor);
	LeftGrippedActor = nullptr;
}

void AGenericVRCharacter::RightGripDown()
{
	if (RightGrippedActor) return;

	//Check for Grabbable actors
	if (bAllowGripping && GrabSphere_R) {
		TArray<AActor*> OverlappingActors;
		GrabSphere_R->GetOverlappingActors(OverlappingActors);
		AActor* NearestActor = GetClosestValidActor(OverlappingActors);
		if (NearestActor) {
			if (IPickupObject::Execute_GrabObject(NearestActor, ReplicatedRightControllerRoot, false)) RightGrippedActor = NearestActor;
		}
		else LogWarning("No Actors with interface found.");
	}
	else if (bAllowGripping && !GrabSphere_R) {
		LogError(GenerateErrorMessage(EVRErrorType::ET_FUNCTION_UNAVAILABLE_COMPONENT_ERROR, "Grab", "Right grab sphere missing."));
	}
}

void AGenericVRCharacter::RightGripUp()
{
	if (RightGrippedActor) IPickupObject::Execute_ReleaseObject(RightGrippedActor);
	RightGrippedActor = nullptr;
}

AActor * AGenericVRCharacter::GetClosestValidActor(TArray<AActor*> InOverlappingActors)
{
	float nearest = 9999.0f;
	AActor* NearestActor = nullptr;
	if (InOverlappingActors.IsValidIndex(0)) {
		for (AActor* actor : InOverlappingActors) {
			float dist = GetDistanceTo(actor);
			if (dist < nearest) {
				NearestActor = actor;
				nearest = dist;
				
			}
		}

		if (NearestActor) return NearestActor;
		else return nullptr;
	}
	
	return nullptr;
}

void AGenericVRCharacter::InitializeHMD()
{
	HMD = GEngine->HMDDevice->GetHMDDeviceType();

	switch (HMD) {
		case EHMDDeviceType::DT_SteamVR:
			LogWarning("HMD Connected: SteamVR");
			break;
		case EHMDDeviceType::DT_OculusRift:
			LogWarning("HMD Connected: Oculus Rift");
			break;
		default:
			LogError(GenerateErrorMessage(EVRErrorType::ET_CUSTOM_ERROR, "", "Invalid HMD Type : Currently the VR Starter Kit only supports SteamVR and OculusRift"));
			break;
	}
}

void AGenericVRCharacter::GetOptionalComponents()
{
	if (bAllowGripping) {
		auto components = GetComponents();
		for (UActorComponent* component : components) {
			if (component->IsA<USphereComponent>()) {
				if (component->ComponentHasTag("LeftGrabSphere")) GrabSphere_L = Cast<USphereComponent>(component);
				else if (component->ComponentHasTag("RightGrabSphere")) GrabSphere_R = Cast<USphereComponent>(component);
			}

			if (component->ComponentHasTag("OptionalMeshComponentRoot")) OptionalCharacterMesh = Cast<USceneComponent>(component);
		}

		if (!GrabSphere_L) LogError(GenerateErrorMessage(EVRErrorType::ET_OPTIONAL_COMPONENT_ERROR, "LeftGrabSphere"));
		if (!GrabSphere_R) LogError(GenerateErrorMessage(EVRErrorType::ET_OPTIONAL_COMPONENT_ERROR, "RightGrabSphere"));
		if (!OptionalCharacterMesh) LogWarning("No OptionalMeshComponentRoot found. Any additional meshes not a child of CharacterMeshRoot will be ignored for replication unless it is a child of a SceneComponent tagged 'OptionalMeshComponentRoot'");
	}
}

FTransform AGenericVRCharacter::GetReplicatedTransform(FTransform ReplicatedTransform, bool bAllowRotation)
{
	FTransform transform;
	transform.SetLocation(ReplicatedTransform.GetLocation());
	if (bAllowRotation) transform.SetRotation(ReplicatedTransform.GetRotation());
	else transform.SetRotation(FQuat::Identity);
	transform.SetScale3D(ReplicatedTransform.GetScale3D());
	return transform;
}

