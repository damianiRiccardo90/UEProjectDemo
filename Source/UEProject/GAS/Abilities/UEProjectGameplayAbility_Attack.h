#pragma once

#include <Abilities/GameplayAbility.h>
#include <CoreMinimal.h>

#include "UEProjectGameplayAbility_Attack.generated.h"


/**
 * A simple attack ability that plays an animation montage and applies damage
 * to targets within a specified distance.
 */
UCLASS()
class UUEProjectGameplayAbility_Attack : public UGameplayAbility
{
    GENERATED_BODY()

public:

    UUEProjectGameplayAbility_Attack(
        const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    //////////////////////////////////////////////////////////////////////////
    // UGameplayAbility overrides

    virtual bool CanActivateAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayTagContainer* SourceTags,
        const FGameplayTagContainer* TargetTags,
        FGameplayTagContainer* OptionalRelevantTags
    ) const override;

    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData
    ) override;
    
    // UGameplayAbility overrides
    //////////////////////////////////////////////////////////////////////////

protected:

    TArray<FHitResult> PerformAttackTrace(const ACharacter* InstigatorCharacter) const;

    void ApplyAttackDamageToTargets(
        const TArray<FHitResult>& HitResults, const FGameplayAbilitySpec* CurrAbilitySpec);

    // Performs the final logic for the ability, such as damaging the target,
    // before calling EndAbility() to wrap up
    UFUNCTION()
    void EndAbilityDirect();

    /** Animation montage to play when the ability is activated */
    UPROPERTY(EditDefaultsOnly, Category = "Gameplay Ability")
    UAnimMontage* AttackMontage;

    /** The maximum distance at which this ability can hit a target */
    UPROPERTY(EditDefaultsOnly, Category = "Gameplay Ability")
    float AttackRange;

    /** Base damage to apply when hitting a target */
    UPROPERTY(EditDefaultsOnly, Category = "Gameplay Ability")
    float BaseAttackDamage;

    /** Damage multiplier to apply to the base attack damage for light attacks. */
    UPROPERTY(EditDefaultsOnly, Category = "Gameplay Ability")
    float LightAttackDamageMultiplier;

    /** Damage multiplier to apply to the base attack damage for heavy attacks. */
    UPROPERTY(EditDefaultsOnly, Category = "Gameplay Ability")
    float HeavyAttackDamageMultiplier;

    /** GameplayEffect to apply for dealing damage to the target. */
    UPROPERTY(EditDefaultsOnly, Category = "Gameplay Ability")
    TSubclassOf<UGameplayEffect> DamageEffect;

private:

    // Tag that is sent along with the gameplay event that triggers the ability
    FGameplayTag AttackType;
};
