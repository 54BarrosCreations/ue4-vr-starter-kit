// Fill out your copyright notice in the Description page of Project Settings.

#include "GenericVRCharacter.h"


// Sets default values
AGenericVRCharacter::AGenericVRCharacter(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer.DoNotCreateDefaultSubobject(ACharacter::MeshComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	VRCameraOrigin = CreateDefaultSubobject<UCameraComponent>(TEXT("VR Camera Origin"));
	VRCameraOrigin->SetupAttachment(RootComponent);
	

}

// Called when the game starts or when spawned
void AGenericVRCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGenericVRCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGenericVRCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

