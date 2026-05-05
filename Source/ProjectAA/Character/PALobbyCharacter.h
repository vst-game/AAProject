#pragma once

#include "CoreMinimal.h"
#include "Character/PABaseCharacter.h"
#include "PALobbyCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class PROJECTAA_API APALobbyCharacter : public APABaseCharacter
{
	GENERATED_BODY()

public:
	APALobbyCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> CameraComponent;
};
