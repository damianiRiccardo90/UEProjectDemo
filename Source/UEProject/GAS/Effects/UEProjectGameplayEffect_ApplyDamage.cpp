#include "UEProjectGameplayEffect_ApplyDamage.h"

#include <GameplayEffectTypes.h>

#include "UEProject/GAS/Attributes/UEProjectAttributeSet_CharacterBase.h"


UUEProjectGameplayEffect_ApplyDamage::UUEProjectGameplayEffect_ApplyDamage()
{
    // Make this effect apply instantly
    DurationPolicy = EGameplayEffectDurationType::Instant;

    // Create a modifier to affect our Health attribute by subtracting from it
    FGameplayModifierInfo ModifierInfo;
    ModifierInfo.Attribute = UUEProjectAttributeSet_CharacterBase::GetHealthAttribute();
    ModifierInfo.ModifierOp = EGameplayModOp::Additive;

    // Configure the magnitude as SetByCaller
    FSetByCallerFloat SetByCaller;
    SetByCaller.DataTag = FGameplayTag::RequestGameplayTag(FName("Data.Damage"));

    // Initialize the modifier magnitude using the SetByCaller constructor
    FGameplayEffectModifierMagnitude ModifierMagnitude(SetByCaller);

    // Attach the magnitude to the info and add the modifier.
    ModifierInfo.ModifierMagnitude = ModifierMagnitude;
    Modifiers.Add(ModifierInfo);
}
