#include "System/PAPlayerController.h"

#include "Component/PAHeroComponent.h"

APAPlayerController::APAPlayerController()
{
	HeroComponent = CreateDefaultSubobject<UPAHeroComponent>(TEXT("HeroComponent"));
}

void APAPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	if (IsValid(HeroComponent))
	{
		HeroComponent->SetupInputComponent(this);
	}
}
