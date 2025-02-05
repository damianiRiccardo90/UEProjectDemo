#include "UEProjectCharacterMovementComponent.h"


UUEProjectCharacterMovementComponent::UUEProjectCharacterMovementComponent(
	const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, DefaultWalkSpeed(200.f)
{
	bOrientRotationToMovement = true;
	bUseControllerDesiredRotation = false;
	RotationRate = FRotator(0.f, 480.f, 0.f);
	// Enable RVO avoidance for better NPC navigation
	bUseRVOAvoidance = true;

	MaxWalkSpeed = DefaultWalkSpeed;
}
