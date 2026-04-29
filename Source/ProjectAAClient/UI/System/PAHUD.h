#pragma once

#include "CoreMinimal.h"
#include "UI/System/PAHUDBase.h"
#include "PAHUD.generated.h"

class UPARootWidget;

UCLASS()
class PROJECTAACLIENT_API APAHUD : public APAHUDBase
{
	GENERATED_BODY()

public:
	virtual void Init() override;
protected:
	
	UPROPERTY(EditAnywhere, Category = "RootWidget")
	TSubclassOf<UPARootWidget> RootWidgetClass;

};
