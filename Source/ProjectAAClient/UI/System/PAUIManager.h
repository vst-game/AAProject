#pragma once

#include "CoreMinimal.h"
#include "UI/System/PAUIManagerBase.h"
#include "PAUIManager.generated.h"


class UUserWidget;

UCLASS()
class PROJECTAACLIENT_API UPAUIManager : public UPAUIManagerBase
{
	GENERATED_BODY()

public:
	virtual void Initialize(TSubclassOf<UUserWidget> RootWidgetClass);
	
protected:
	
	UPROPERTY(Transient)
	TObjectPtr<UUserWidget> RootWidget;
};

