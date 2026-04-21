#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PAPlayerController.generated.h"

class UPAHeroComponent;

UCLASS()
class PROJECTAA_API APAPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	APAPlayerController();
	
protected:
	
	virtual void SetupInputComponent() override;
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UPAHeroComponent> HeroComponent;
};
