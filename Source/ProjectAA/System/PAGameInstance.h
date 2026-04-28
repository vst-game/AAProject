#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PAGameInstance.generated.h"

class UPAUIManagerBase;

UCLASS()
class PROJECTAA_API UPAGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	
	virtual void Init() override;
	
protected:
	
	UPROPERTY(Transient)
	TObjectPtr<UPAUIManagerBase> UIManager;
};
