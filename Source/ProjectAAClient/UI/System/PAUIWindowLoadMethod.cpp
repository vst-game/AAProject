#include "UI/System/PAUIWindowLoadMethod.h"
#include "UI/UI/PACommonActivatableWidget.h"

UClass* UPAUIWindowLoadMethod::LoadAssetClass_Implementation(UClass* ClassName)
{
	TSoftClassPtr<UPACommonActivatableWidget>* SearchResult = WindowMap.Find(ClassName->GetFName());

	if (nullptr == SearchResult)
	{
		return nullptr;
	}

	// 동기적으로 로딩
	return SearchResult->LoadSynchronous();
}