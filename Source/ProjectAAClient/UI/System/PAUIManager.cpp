#include "UI/System/PAUIManager.h"

#include "System/PAGameInstance.h"
#include "UI/System/PAHUDBase.h"
#include "UI/System/PARootWidget.h"

UPAUIManager* UPAUIManager::Get(UObject* Context)
{
	UWorld* World = Context->GetWorld();
	
	check(World);
	
	UPAGameInstance* GameInstance = Cast<UPAGameInstance>(World->GetGameInstance());
	
	ensure(GameInstance);
	
	return Cast<UPAUIManager>(GameInstance->GetUIManager());
}

void UPAUIManager::Initialize(APAHUDBase* HUDBase, TSubclassOf<UPARootWidget> RootWidgetClass)
{
	RootWidget = CreateWidget<UPARootWidget>(HUDBase->GetOwningPlayerController(), RootWidgetClass);

	if (IsValid(RootWidget))
	{
		RootWidget->AddToViewport();
	}
	
	HUD = HUDBase;
}
