// Fill out your copyright notice in the Description page of Project Settings.

#include "GenericVRPawn.h"


// Sets default values
AGenericVRPawn::AGenericVRPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Default Scene Root"));
	RootComponent = DefaultSceneRoot;
	VROrigin = CreateDefaultSubobject<USceneComponent>(TEXT("VR Origin"));
	VROrigin->SetupAttachment(DefaultSceneRoot);
	DefaultCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Default Camera"));
	DefaultCamera->SetupAttachment(VROrigin);
}

// Called when the game starts or when spawned
void AGenericVRPawn::BeginPlay()
{
	Super::BeginPlay();
	if (!bDoNotExecuteBaseBeginPlay) {
		if (LocomotionType != EVRLocomotionType::LT_Gamepad) {
			if (!LeftController) SpawnMotionController(EControllerHand::Left);
			if (!RightController) SpawnMotionController(EControllerHand::Right);
			if (!bAllowMotionControllerWidgetInteraction) {
				//Setting these to true before setup will ensure that the interaction components are disabled
				LeftController->bFirstTimeSetupComplete = true;
				RightController->bFirstTimeSetupComplete = true;
			}
		}
	}
}

// Called every frame
void AGenericVRPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bDoNotExecuteBaseTick) {
		if (!ActivePointerController && bAllowMotionControllerWidgetInteraction) InitControllerInteraction();
	}
}

// Called to bind functionality to input
void AGenericVRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("LeftTrigger", EInputEvent::IE_Pressed, this, &AGenericVRPawn::LeftTriggerDown);
	PlayerInputComponent->BindAction("LeftTrigger", EInputEvent::IE_Released, this, &AGenericVRPawn::LeftTriggerUp);
	PlayerInputComponent->BindAction("RightTrigger", EInputEvent::IE_Pressed, this, &AGenericVRPawn::RightTriggerDown);
	PlayerInputComponent->BindAction("RightTrigger", EInputEvent::IE_Released, this, &AGenericVRPawn::RightTriggerUp);
}

void AGenericVRPawn::SpawnMotionController(EControllerHand Hand, bool bInvertScale)
{
	//Return if no Motion Controller Template
	if (!MotionControllerTemplate->IsValidLowLevel()) {
		UE_LOG(LogTemp, Error, TEXT("Motion Controller Template is not set. Cannot spawn motion controllers."));
		return;
	}

	//Spawn Controller
	FActorSpawnParameters Params;
	Params.Owner = this;
	FTransform SpawnTransform = FTransform::Identity;
	auto a = GetWorld()->SpawnActor<AGenericMotionController>(MotionControllerTemplate, SpawnTransform, Params);
	
	//Set Scale and Assign Variables
	if (a) {
		if (bInvertScale) a->SetActorScale3D(a->GetActorScale3D() * -1);

		//Attach Controller to Pawn
		FAttachmentTransformRules AttachRules = FAttachmentTransformRules::SnapToTargetNotIncludingScale;
		a->AttachToComponent(VROrigin, AttachRules);

		//Set Controller Variable
		if (Hand == EControllerHand::Left) {
			LeftController = a;
			LeftController->MotionController->Hand = EControllerHand::Left;
		}
		else if (Hand == EControllerHand::Right) {
			RightController = a;
			RightController->MotionController->Hand = EControllerHand::Right;
			RightController->InteractionComponent->VirtualUserIndex = 1;
			RightController->InteractionComponent->PointerIndex = 1;
		}
		else {
			UE_LOG(LogActor, Error, TEXT("Invalid Hand type. Spawn controller will be marked for kill."));
			a->Destroy();
		}
	}
	else UE_LOG(LogTemp, Error, TEXT("Motion controller failed to spawn."));
}

void AGenericVRPawn::InitControllerInteraction()
{
	bool LeftControllerUIHit = LeftController->PollForUIHit();
	bool RightControllerUIHit = RightController->PollForUIHit();

	if (LeftControllerUIHit) {
		SetActivePointerController(LeftController);
		LeftController->bFirstTimeSetupComplete = true;
		RightController->bFirstTimeSetupComplete = true;
	} else if (RightControllerUIHit) {
		SetActivePointerController(RightController);
		LeftController->bFirstTimeSetupComplete = true;
		RightController->bFirstTimeSetupComplete = true;
	} 
}

void AGenericVRPawn::SetActivePointerController(AGenericMotionController * NewActive)
{
	if (ActivePointerController) ActivePointerController->bPointerActive = false;
	ActivePointerController = NewActive;
	if (ActivePointerController) ActivePointerController->bPointerActive = true;
}

void AGenericVRPawn::SetActiveTeleporterController(AGenericMotionController * NewActive)
{
	if (ActiveTeleportController) ActiveTeleportController->DeactivateTeleporter();
	ActiveTeleportController = NewActive;
	if (ActiveTeleportController) ActiveTeleportController->ActivateTeleporter();
}

void AGenericVRPawn::LeftTriggerDown()
{
	if (ActivePointerController && ActivePointerController == RightController) SetActivePointerController(LeftController);
	LeftController->PressInteract();
}

void AGenericVRPawn::LeftTriggerUp()
{
	LeftController->ReleaseInteract();
}

void AGenericVRPawn::RightTriggerDown()
{
	if (ActivePointerController && ActivePointerController == LeftController) SetActivePointerController(RightController);
	RightController->PressInteract();
}

void AGenericVRPawn::RightTriggerUp()
{
	RightController->ReleaseInteract();
}

