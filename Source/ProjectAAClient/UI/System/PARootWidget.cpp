#include "UI/System/PARootWidget.h"
#include "PACommonActivatableWidgetStack.h"
#include "PAUIWindowLoadMethod.h"
#include "UI/UI/PACommonActivatableWidget.h"
#include "Components/PanelWidget.h"

void UPARootWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	TArray<UWidget*> ChildWidgets = Root->GetAllChildren();
	
	for (UWidget* ChildWidget : ChildWidgets)
	{
		UPanelWidget* Widget = Cast<UPanelWidget>(ChildWidget);
		
		if (IsValid(Widget))
		{
			UPACommonActivatableWidgetStack* WidgetStack = Cast<UPACommonActivatableWidgetStack>(Widget->GetChildAt(0));
			
			if (IsValid(WidgetStack))
			{
				LayerMap.Add(WidgetStack->GetLayerType(), WidgetStack);
			}
		}
	}
}

UPACommonActivatableWidget* UPARootWidget::GetOrCreateInstance(UClass* Class, const FGameplayTag& LayerTag)
{
	return GetOrCreateInstanceInternal(Class, LayerTag);
}

UPACommonActivatableWidget* UPARootWidget::GetOrCreateInstanceInternal(UClass* Class, const FGameplayTag& LayerTag)
{
	TObjectPtr<UCommonActivatableWidgetStack>* WidgetStack = LayerMap.Find(LayerTag);

	if (nullptr == WidgetStack)
	{
		return nullptr;
	}
	UPACommonActivatableWidget* ReturnWidget = nullptr;

	FPAPoolContainer* Pool = PoolingContainer.Find(Class->GetFName());

	if (nullptr != Pool)
	{
		int32 LastIndex = Pool->Container.Num() - 1;

		if (LastIndex >= 0)
		{
			//0보다 크면 마지막껄 꺼낸다.
			ReturnWidget = Pool->Container[LastIndex];
			Pool->Container.RemoveSingleSwap(ReturnWidget);
		}
	
		(*WidgetStack)->AddWidgetInstance(*ReturnWidget);
	}

	if (ReturnWidget == nullptr)
	{
		UPAUIWindowLoadMethod* WindowLoadMethodCDO = WindowLoadMethodClass->GetDefaultObject<UPAUIWindowLoadMethod>();

		if (false == IsValid(WindowLoadMethodCDO))
		{
			return nullptr;
		}

		TSubclassOf<UPACommonActivatableWidget> ActivatableWidgetClass = WindowLoadMethodCDO->LoadAssetClass(Class);

		if (false == IsValid(ActivatableWidgetClass))
		{
			return nullptr;
		}

		ReturnWidget = Cast<UPACommonActivatableWidget>((*WidgetStack)->AddWidget(ActivatableWidgetClass));
	}
	
	return ReturnWidget;
}
