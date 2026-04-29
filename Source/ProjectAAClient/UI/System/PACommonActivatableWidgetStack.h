#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "PACommonActivatableWidgetStack.generated.h"

UCLASS()
class PROJECTAACLIENT_API UPACommonActivatableWidgetStack : public UCommonActivatableWidgetStack
{
	GENERATED_BODY()

public:
	
	const FGameplayTag& GetLayerType() { return LayerTag; }
protected:
	
	UPROPERTY(EditAnywhere, Category = "Layer|Category")
	FGameplayTag LayerTag;
	
};
