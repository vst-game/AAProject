#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/PAInteractable.h"
#include "PACustomBox.generated.h"

// 인터랙션 가능한 코스튬 박스
// 커스텀을 위한 UI를 띄우는 용도
UCLASS()
class PROJECTAA_API APACustomBox : public AActor, public IPAInteractable
{
	GENERATED_BODY()

public:
	APACustomBox();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Interact(AActor* Interactor) override;
	virtual FText GetInteractionText() const override;

private:
	void RotateCameraToFront(class APALobbyCharacter* LobbyCharacter);
	
	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
};
