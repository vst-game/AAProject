#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PAHeroComponent.generated.h"

class APlayerController;
class UInputMappingContext;
class UInputAction;

struct FInputActionValue;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTAA_API UPAHeroComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPAHeroComponent();

	void SetupInputComponent(APlayerController* InPlayerController);
	
protected:

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	
protected:
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Move;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Look;
	
protected:
	UPROPERTY(Transient)
	TWeakObjectPtr<APlayerController> PlayerController;
	
	UPROPERTY(Transient)
	TWeakObjectPtr<APawn> OwnerPawn;
};
