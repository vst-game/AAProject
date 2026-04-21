#pragma once

#include "CoreMinimal.h"
#include "Character/PABaseCharacter.h"
#include "PACharacter.generated.h"
class UCameraComponent;

UCLASS()
class PROJECTAA_API APACharacter : public APABaseCharacter
{
	GENERATED_BODY()
	
public:
	APACharacter();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = true))
	TObjectPtr<UCameraComponent> CameraComponent;
};
