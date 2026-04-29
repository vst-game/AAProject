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
	
	UPAUIManagerBase* GetUIManager() { return UIManager; }
	
protected:
	
	UPROPERTY(EditAnywhere, Category = "Class|UI")
	TSubclassOf<UPAUIManagerBase> UIManagerClass;
	
protected:
	UPROPERTY(Transient)
	TObjectPtr<UPAUIManagerBase> UIManager;
};
