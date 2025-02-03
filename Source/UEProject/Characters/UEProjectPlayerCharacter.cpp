#include "UEProjectPlayerCharacter.h"

#include <Components/CapsuleComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <GameFramework/GameplayCameraComponent.h>


AUEProjectPlayerCharacter::AUEProjectPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, GameplayCameraComponent(nullptr)
{
    GameplayCameraComponent = 
		CreateDefaultSubobject<UGameplayCameraComponent>(TEXT("GameplayCameraComponent"));
	GameplayCameraComponent->SetupAttachment(RootComponent);
	GameplayCameraComponent->bAutoActivate = true;
	GameplayCameraComponent->AutoActivateForPlayer = EAutoReceiveInput::Player0;

	if (UCharacterMovementComponent* const CharacterMovementComponent = GetCharacterMovement())
	{
		// Note: For faster iteration times these variables, and many more, can be tweaked in the 
		// Character Blueprint instead of recompiling to adjust them
		CharacterMovementComponent->RotationRate = FRotator(0.f, 500.f, 0.f);
		CharacterMovementComponent->JumpZVelocity = 700.f;
		CharacterMovementComponent->AirControl = 0.35f;
		CharacterMovementComponent->MaxWalkSpeed = 500.f;
		CharacterMovementComponent->MinAnalogWalkSpeed = 20.f;
		CharacterMovementComponent->BrakingDecelerationWalking = 2000.f;
		CharacterMovementComponent->BrakingDecelerationFalling = 1500.f;
	}

	if (UCapsuleComponent* const CapsuleComp = GetCapsuleComponent())
	{
		// Set size for collision capsule
		CapsuleComp->InitCapsuleSize(42.f, 96.f);
	}
}

UGameplayCameraComponent* AUEProjectPlayerCharacter::GetGameplayCameraComponent() const
{
	return GameplayCameraComponent;
}
