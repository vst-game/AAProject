#include "Component/PAHeroComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

UPAHeroComponent::UPAHeroComponent()
{

}

void UPAHeroComponent::SetupInputComponent(APlayerController* InPlayerController)
{
	if (false == IsValid(InPlayerController))
	{
		return;
	}

	PlayerController = InPlayerController;
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(InPlayerController->GetLocalPlayer());
	
	ensure(Subsystem);
	
	Subsystem->AddMappingContext(DefaultMappingContext, 0);
	
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InPlayerController->InputComponent);
	
	if (IsValid(EnhancedInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ThisClass::Move);
		EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ThisClass::Look);
	}
}

void UPAHeroComponent::Move(const FInputActionValue& Value)
{
	if (false == PlayerController.IsValid())
	{
		return;
	}
	
	if (false == OwnerPawn.IsValid())
	{
		return;
	}
	
	FVector2D MovementVector = Value.Get<FVector2D>();
	
	const FRotator Rotation = PlayerController->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	OwnerPawn->AddMovementInput(ForwardDirection, MovementVector.Y);
	OwnerPawn->AddMovementInput(RightDirection, MovementVector.X);
}

void UPAHeroComponent::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (PlayerController.IsValid())
	{
		if (OwnerPawn.IsValid())
		{
			OwnerPawn->AddControllerYawInput(LookAxisVector.X);
			OwnerPawn->AddControllerPitchInput(LookAxisVector.Y);
		}
	}
}

void UPAHeroComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (PlayerController.IsValid())
	{
		OwnerPawn = Cast<APawn>(PlayerController->GetPawn());
	}
}
