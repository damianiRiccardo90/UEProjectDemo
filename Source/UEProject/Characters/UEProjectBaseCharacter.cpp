#include "UEProjectBaseCharacter.h"

#include <AbilitySystemComponent.h>
#include <Components/CapsuleComponent.h>

#include "UEProject/Characters/Movement/UEProjectCharacterMovementComponent.h"
#include "UEProject/GAS/Attributes/UEProjectAttributeSet_CharacterBase.h"


AUEProjectBaseCharacter::AUEProjectBaseCharacter(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer
        .SetDefaultSubobjectClass<UUEProjectCharacterMovementComponent>(
            ACharacter::CharacterMovementComponentName
        )
    )
{
    // Don't let controller override rotation
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

    AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));

    BaseAttributeSet = 
        CreateDefaultSubobject<UUEProjectAttributeSet_CharacterBase>(TEXT("BaseAttributeSet"));

    // Register the attribute set with the ASC
    if (AbilitySystem && BaseAttributeSet)
    {
        AbilitySystem->AddAttributeSetSubobject(BaseAttributeSet);
    }
}

UAbilitySystemComponent* AUEProjectBaseCharacter::GetAbilitySystem() const
{
    return AbilitySystem;
}

UUEProjectAttributeSet_CharacterBase* AUEProjectBaseCharacter::GetBaseAttributeSet() const
{
	return BaseAttributeSet;
}

UUEProjectCharacterMovementComponent* AUEProjectBaseCharacter::GetCustomMovementComponent() const
{
    return Cast<UUEProjectCharacterMovementComponent>(GetMovementComponent());
}
