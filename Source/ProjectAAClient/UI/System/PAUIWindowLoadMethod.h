#pragma once

#include "CoreMinimal.h"
#include "UI/System/PAUIWindowLoadMethodBase.h"
#include "PAUIWindowLoadMethod.generated.h"

class UPACommonActivatableWidget;

UCLASS()
class PROJECTAACLIENT_API UPAUIWindowLoadMethod : public UPAUIWindowLoadMethodBase
{
	GENERATED_BODY()

public:
	
	virtual UClass* LoadAssetClass_Implementation(UClass* Class) override;
	
protected:
	
	UPROPERTY(EditAnywhere, Category = "Window | UI")
	TMap<FName, TSoftClassPtr<UPACommonActivatableWidget>> WindowMap;
};
