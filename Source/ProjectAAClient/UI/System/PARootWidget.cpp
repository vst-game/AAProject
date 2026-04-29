#include "UI/System/PARootWidget.h"
#include "PACommonActivatableWidgetStack.h"
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
