#pragma once

#include <AbilitySystemComponent.h>
#include <AttributeSet.h>
#include <CoreMinimal.h>

#include "UEProjectAttributeSet_CharacterBase.generated.h"


/** Attribute set that holds basic character attributes. */
UCLASS()
class UUEProjectAttributeSet_CharacterBase : public UAttributeSet
{
    GENERATED_BODY()

public:

    UUEProjectAttributeSet_CharacterBase(
        const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    //////////////////////////////////////////////////////////////////////////
    // UAttributeSet overrides

    /* Called just before an attribute is changed */
    virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

    /* Called after changes have been applied to attributes in an Effect's execution phase */
    virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

    // UAttributeSet overrides
    //////////////////////////////////////////////////////////////////////////

    // Health accessor macros (optional, but recommended)

    // Provides a type-safe way to obtain the FGameplayAttribute
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UUEProjectAttributeSet_CharacterBase, Health);
    // Enables an InitHealth(...) function you call to set a default
	GAMEPLAYATTRIBUTE_VALUE_INITTER(Health);

    /**
     * Provides a blueprint-friendly way to get the current health value
     * after all modifiers have been applied.
     */
    UFUNCTION(BlueprintCallable, Category="Health")
    float GetHealth() const;

    /**
     * Sets the base value for Health (not the "Current" value or "Bonus"),
     * ensuring the ability system accounts for all active GameplayEffects.
     */
    UFUNCTION(BlueprintCallable, Category="Health")
    void SetHealth(float NewVal);

protected:

    /* Current Health attribute */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health")
    FGameplayAttributeData Health;
};
