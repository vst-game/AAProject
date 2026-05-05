#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "PAUIManagerBase.generated.h"


class UUserWidget;
class APAHUDBase;
class UPARootWidget;
class UPACommonActivatableWidget;

UCLASS(Abstract, Blueprintable)
class PROJECTAA_API UPAUIManagerBase : public UObject
{
	GENERATED_BODY()

public:
	virtual void Initialize(APAHUDBase* HUDBase, TSubclassOf<UPARootWidget> RootWidgetClass) { };
	
	UPACommonActivatableWidget* OpenWindow(UClass* WidgeClass, FGameplayTag LayerTag) { return OpenWindowInternalImpl(WidgeClass, LayerTag); }

protected:
	virtual UPACommonActivatableWidget* OpenWindowInternalImpl(UClass* WidgetClass, FGameplayTag LayerTag) { return nullptr; }
	
};
