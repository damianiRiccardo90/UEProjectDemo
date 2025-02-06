#include "UEProjectPlayerCharacter.h"

#include <Components/CapsuleComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <GameFramework/GameplayCameraComponent.h>
#include <Perception/AIPerceptionStimuliSourceComponent.h>
#include <Perception/AISense_Sight.h>
#include <Perception/AISense_Touch.h>


AUEProjectPlayerCharacter::AUEProjectPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, GameplayCameraComponent(nullptr)
	, PerceptionStimuliSource(nullptr)
{
    GameplayCameraComponent = 
		CreateDefaultSubobject<UGameplayCameraComponent>(TEXT("GameplayCameraComponent"));
	GameplayCameraComponent->SetupAttachment(RootComponent);
	GameplayCameraComponent->bAutoActivate = true;
	GameplayCameraComponent->AutoActivateForPlayer = EAutoReceiveInput::Player0;

	PerceptionStimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(
		TEXT("PerceptionStimuliSource"));
	if (PerceptionStimuliSource)
	{
		PerceptionStimuliSource->bAutoRegister = true;
		PerceptionStimuliSource->RegisterForSense(UAISense_Sight::StaticClass());
		PerceptionStimuliSource->RegisterForSense(UAISense_Touch::StaticClass());
	}

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

FGenericTeamId AUEProjectPlayerCharacter::GetGenericTeamId() const
{
	return 0;
}

ETeamAttitude::Type AUEProjectPlayerCharacter::GetTeamAttitudeTowards(const AActor& Other) const
{
	const IGenericTeamAgentInterface* const OtherTeamAgent = 
		Cast<IGenericTeamAgentInterface>(&Other);
	if (!OtherTeamAgent) return ETeamAttitude::Neutral;

	const uint8 OtherAgentTeamID = OtherTeamAgent->GetGenericTeamId().GetId();

	if (OtherAgentTeamID == 0) return ETeamAttitude::Friendly;
	if (OtherAgentTeamID == 1) return ETeamAttitude::Hostile;

	return ETeamAttitude::Neutral;
}

UGameplayCameraComponent* AUEProjectPlayerCharacter::GetGameplayCameraComponent() const
{
	return GameplayCameraComponent;
}
