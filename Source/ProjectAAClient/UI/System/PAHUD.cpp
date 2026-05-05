#include "UI/System/PAHUD.h"

#include "PAUIManager.h"
#include "UI/UI/HUD/PAHUDLobby.h"

void APAHUD::Init()
{
	Super::Init();
	
	UPAUIManager* UIManager = UPAUIManager::Get(this);
	
	ensure(UIManager);
	
	UIManager->Initialize(this, RootWidgetClass);
	
	UPAHUDLobby::OpenWindow(UIManager, UPAHUDLobby::StaticClass());
}
