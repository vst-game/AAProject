#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PAHUDBase.generated.h"

UCLASS()
class PROJECTAA_API APAHUDBase : public AHUD
{
	GENERATED_BODY()

public:
	
	virtual void BeginPlay() override;
protected:
	virtual void Init() { }
protected:
	

};
