#include "System/PAGameInstance.h"

#include "UI/System/PAUIManagerBase.h"

void UPAGameInstance::Init()
{
	Super::Init();
	
	UIManager = NewObject<UPAUIManagerBase>(this);
}
