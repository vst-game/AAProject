#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PABaseCharacter.generated.h"

UCLASS()
class PROJECTAA_API APABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APABaseCharacter();

protected:
	virtual void BeginPlay() override;

};
