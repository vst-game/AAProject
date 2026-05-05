#pragma once

#include "CoreMinimal.h"
#include "PAUIWindowLoadMethodBase.generated.h"

UCLASS(Blueprintable)
class PROJECTAA_API UPAUIWindowLoadMethodBase : public UObject
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintNativeEvent)
	UClass* LoadAssetClass(UClass* Class);
	
	virtual UClass* LoadAssetClass_Implementation(UClass* Class) { return nullptr; }

};