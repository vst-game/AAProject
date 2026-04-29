#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameplayTagContainer.h"
#include "PARootWidget.generated.h"

class UCommonActivatableWidgetStack;

UCLASS()
class PROJECTAACLIENT_API UPARootWidget : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	
	virtual void NativeConstruct() override;
	
protected:
	
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UPanelWidget> Root;
	
	UPROPERTY()
	TMap<FGameplayTag, TObjectPtr<UCommonActivatableWidgetStack>> LayerMap;
	
};
