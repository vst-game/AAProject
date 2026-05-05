#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/UI/PACommonActivatableWidget.h"
#include "UI/System/PARootWidget.h"
#include "UI/System/PAUIManagerBase.h"
#include "PAHUDPageBase.generated.h"

UCLASS()
class PROJECTAACLIENT_API UPAHUDPageBase : public UPACommonActivatableWidget
{
	GENERATED_BODY()
	
public:
	
	static UPAHUDPageBase* OpenWindow(UPAUIManagerBase* UIManager, UClass* Class)
	{
		return Cast<UPAHUDPageBase>(UIManager->OpenWindow(Class, FGameplayTag::RequestGameplayTag(TEXT("UI.Layer.HUD"))));
	}
};
