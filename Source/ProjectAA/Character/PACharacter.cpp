#include "Character/PACharacter.h"
#include "Camera/CameraComponent.h"

APACharacter::APACharacter()
	: Super()
{
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(GetMesh(), TEXT("head"));
}
