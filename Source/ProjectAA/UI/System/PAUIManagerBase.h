#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PAUIManagerBase.generated.h"


class UUserWidget;
class APAHUDBase;
class UPARootWidget;

UCLASS(Abstract)
class PROJECTAA_API UPAUIManagerBase : public UObject
{
	GENERATED_BODY()

public:
	virtual void Initialize(APAHUDBase* HUDBase, TSubclassOf<UPARootWidget> RootWidgetClass) { };
};
