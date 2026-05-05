#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/System/PAUIManagerBase.h"
#include "PAUIManager.generated.h"

class UPACommonActivatableWidget;
class UPARootWidget;
class APAHUDBase;

UCLASS()
class PROJECTAACLIENT_API UPAUIManager : public UPAUIManagerBase
{
	GENERATED_BODY()

public:
	static UPAUIManager* Get(UObject* Context);
	
	virtual void Initialize(APAHUDBase* HUDBase, TSubclassOf<UPARootWidget> RootWidgetClass) override;
protected:

	virtual UPACommonActivatableWidget* OpenWindowInternalImpl(UClass* WidgetClass, FGameplayTag LayerTag) override;;

protected:
	
	UPROPERTY(Transient)
	TObjectPtr<UPARootWidget> RootWidget;
	
	UPROPERTY(Transient)
	TObjectPtr<APAHUDBase> HUD;
	
	UPROPERTY(Transient)
	TArray<TWeakObjectPtr<UPACommonActivatableWidget>> ActiveWidgets;
	
};

