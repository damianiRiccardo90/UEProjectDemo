#include "UEProjectBaseCharacter.h"

#include <AbilitySystemComponent.h>

#include "UEProject/GAS/Attributes/UEProjectAttributeSet_CharacterBase.h"


AUEProjectBaseCharacter::AUEProjectBaseCharacter()
	: AbilitySystem(nullptr)
	, BaseAttributeSet(nullptr)
{
	AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));

	// Create the attribute set and make sure it's "owned" by the ASC, so it can replicate properly.
    BaseAttributeSet = CreateDefaultSubobject<UUEProjectAttributeSet_CharacterBase>(TEXT("BaseAttributeSet"));

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
