#include "Gimmick/PACustomBox.h"
#include "Character/PALobbyCharacter.h"
#include "GameFramework/PlayerController.h"

APACustomBox::APACustomBox()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	
	RootComponent = StaticMeshComponent;
}

void APACustomBox::BeginPlay()
{
	Super::BeginPlay();
}

void APACustomBox::Interact(AActor* Interactor)
{
	APALobbyCharacter* LobbyCharacter = Cast<APALobbyCharacter>(Interactor);
	if (false == IsValid(LobbyCharacter))
	{
		return;
	}

	RotateCameraToFront(LobbyCharacter);

	// TODO: 코스튬 UI 오픈
}

FText APACustomBox::GetInteractionText() const
{
	return FText::FromString(TEXT("코스튬 변경"));
}

void APACustomBox::RotateCameraToFront(APALobbyCharacter* LobbyCharacter)
{
	APlayerController* PC = Cast<APlayerController>(LobbyCharacter->GetController());
	if (false == IsValid(PC))
	{
		return;
	}

	// 캐릭터 앞모습 = 캐릭터 뒤에서 바라보는 방향의 반대 = 캐릭터 ForwardVector 방향에서 바라봄
	// SpringArm은 ControlRotation을 따르므로, ControlRotation의 Yaw를 캐릭터 정면 방향으로 설정
	FRotator FrontRotation = LobbyCharacter->GetActorForwardVector().Rotation();
	FrontRotation.Pitch = PC->GetControlRotation().Pitch;
	FrontRotation.Roll = 0.f;

	PC->SetControlRotation(FrontRotation);
	PC->SetIgnoreLookInput(true);
}
