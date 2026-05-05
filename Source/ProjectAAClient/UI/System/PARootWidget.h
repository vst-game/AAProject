#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameplayTagContainer.h"
#include "PARootWidget.generated.h"

class UCommonActivatableWidgetStack;
class UPACommonActivatableWidget;
class UPAUIWindowLoadMethod;

USTRUCT()
struct FPAPoolContainer
{
	GENERATED_USTRUCT_BODY()
public:

	UPROPERTY(Transient)
	TArray<TObjectPtr<UPACommonActivatableWidget>> Container;
};

UCLASS()
class PROJECTAACLIENT_API UPARootWidget : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	
	virtual void NativeConstruct() override;

	virtual UPACommonActivatableWidget* GetOrCreateInstance(UClass* Class, const FGameplayTag& LayerTag);

protected:

	UPACommonActivatableWidget* GetOrCreateInstanceInternal(UClass* Class, const FGameplayTag& LayerTag);

protected:
	
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UPAUIWindowLoadMethod> WindowLoadMethodClass;
	
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UPanelWidget> Root;
	
protected:
	
	UPROPERTY(Transient)
	TMap<FGameplayTag, TObjectPtr<UCommonActivatableWidgetStack>> LayerMap;

	UPROPERTY(Transient)
	TMap<FName, FPAPoolContainer> PoolingContainer;
};
