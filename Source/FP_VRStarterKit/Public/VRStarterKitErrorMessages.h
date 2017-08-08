#pragma once

#include "CoreMinimal.h"
#include "VRStarterKitErrorMessages.generated.h"

UENUM(BlueprintType)
enum class EVRErrorType : uint8 {
	ET_OPTIONAL_COMPONENT_ERROR,
	ET_IMPROPER_TYPE_ERROR
};

static FString GenerateVRErrorMessage(EVRErrorType ErrorType, FString ObjectName = "Object", FString AdditionalInfo = "") {
	FString Message;
	switch (ErrorType) {
		case EVRErrorType::ET_OPTIONAL_COMPONENT_ERROR:
			Message = FString::Printf(TEXT("%s is flagged as a required component by its parent class. Please attach a component of the same name or remove the flag in the class options. %s"), *ObjectName, *AdditionalInfo);
			break;
		case EVRErrorType::ET_IMPROPER_TYPE_ERROR:
			Message = FString::Printf(TEXT("Object is not of proper type. Object must inherit from %s to function properly. %s"), *ObjectName, *AdditionalInfo);
			break;
		default:
			Message = FString::Printf(TEXT("UNKNOWN ERROR TYPE: %s"), *AdditionalInfo);
			break;

	}
	return Message;
}

static void LogError(FString message) {
	UE_LOG(LogTemp, Error, TEXT("VR Starter Kit: %s"), *message)
}

static void LogWarning(FString message) {
	UE_LOG(LogTemp, Warning, TEXT("VR Starter Kit: %s"), *message)
}

UCLASS()
class UDummyClass : public UObject {
	GENERATED_BODY()
};