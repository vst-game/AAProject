#include "UI/System/PAHUD.h"

#include "PAUIManager.h"

void APAHUD::Init()
{
	Super::Init();
	
	UPAUIManager* UIManager = UPAUIManager::Get(this);
	
	ensure(UIManager);
	
	UIManager->Initialize(this, RootWidgetClass);
}
