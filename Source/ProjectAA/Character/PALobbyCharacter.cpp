#include "Character/PALobbyCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

APALobbyCharacter::APALobbyCharacter()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 300.f;
	SpringArm->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;
}
