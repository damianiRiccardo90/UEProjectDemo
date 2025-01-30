#include "UEProjectAttributeSet_CharacterBase.h"

#include "GameplayEffectExtension.h"


UUEProjectAttributeSet_CharacterBase::UUEProjectAttributeSet_CharacterBase()
{
    // Initialize Health via the macro-generated InitHealth function.
    InitHealth(100.0f);
}

void UUEProjectAttributeSet_CharacterBase::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
    Super::PreAttributeChange(Attribute, NewValue);

    // Keep Health above 0 before the change is finalized
    if (Attribute == GetHealthAttribute())
    {
        NewValue = FMath::Max(NewValue, 0.0f);
    }
}

void UUEProjectAttributeSet_CharacterBase::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);

    // After an effect's execution modifies Health, clamp it as needed
    if (Data.EvaluatedData.Attribute == GetHealthAttribute())
    {
        // Ensure it doesn't go below 0
        const float NewHealth = FMath::Max(GetHealth(), 0.0f);
        SetHealth(NewHealth);

        // TODO: If Health hits 0, handle “death” or other logic here.
    }
}

float UUEProjectAttributeSet_CharacterBase::GetHealth() const
{
    // Return the final Current value for Health
    return Health.GetCurrentValue();
}

void UUEProjectAttributeSet_CharacterBase::SetHealth(float NewVal)
{
    // Do not accept values lower than zero, or higher than 100
    NewVal = FMath::Clamp(NewVal, 0.f, 100.f);

    if (UAbilitySystemComponent* const ASC = GetOwningAbilitySystemComponent())
    {
        // Setting the base value ensures that applied modifiers
        // remain valid. The engine then updates CurrentValue properly.
        ASC->SetNumericAttributeBase(GetHealthAttribute(), NewVal);
    }
}
