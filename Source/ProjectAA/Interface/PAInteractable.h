#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PAInteractable.generated.h"

UINTERFACE(MinimalAPI)
class UPAInteractable : public UInterface
{
	GENERATED_BODY()
};

class PROJECTAA_API IPAInteractable
{
	GENERATED_BODY()

public:
	virtual void Interact(AActor* Interactor) = 0;
	virtual bool CanInteract(AActor* Interactor) const { return true; }
	virtual FText GetInteractionText() const { return FText::GetEmpty(); }
};
