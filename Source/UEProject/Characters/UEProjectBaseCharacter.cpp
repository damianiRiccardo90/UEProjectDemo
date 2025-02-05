#include "UEProjectBaseCharacter.h"

#include <AbilitySystemComponent.h>
#include <Components/CapsuleComponent.h>

#include "UEProject/Characters/Movement/UEProjectCharacterMovementComponent.h"
#include "UEProject/GAS/Abilities/UEProjectGameplayAbilitySet.h"
#include "UEProject/GAS/Attributes/UEProjectAttributeSet_CharacterBase.h"
#include "UEProject/GAS/Effects/UEProjectGameplayEffect_ApplyDamage.h"
#include "UEProject/GAS/UEProjectGameplayTagsLibrary.h"


AUEProjectBaseCharacter::AUEProjectBaseCharacter(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer
        .SetDefaultSubobjectClass<UUEProjectCharacterMovementComponent>(
            ACharacter::CharacterMovementComponentName
        )
    )
    , AbilitySystem(nullptr)
    , BaseAttributeSet(nullptr)
    , DefaultAbilitySet(nullptr)
{
    // Don't let controller override rotation
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

    AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));

    BaseAttributeSet = 
        CreateDefaultSubobject<UUEProjectAttributeSet_CharacterBase>(TEXT("BaseAttributeSet"));

    if (AbilitySystem)
    {
        // Register the attribute set with the ASC
        AbilitySystem->AddAttributeSetSubobject(BaseAttributeSet);

        // Register callback for gameplay effects applied to itself
        AbilitySystem->OnGameplayEffectAppliedDelegateToSelf.AddUObject(
            this, &ThisClass::OnGameplayEffectApplied);
    }
}

void AUEProjectBaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    // Grant default abilities (only if the actor has the authority in a networked game)
    if (HasAuthority() && AbilitySystem && DefaultAbilitySet)
    {
        for (const FGrantAbility& Grant : DefaultAbilitySet->GrantedAbilities)
        {
            if (Grant.AbilityClass)
            {
                const FGameplayAbilitySpec Spec(Grant.AbilityClass);
                AbilitySystem->GiveAbility(Spec);
            }
        }
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

void AUEProjectBaseCharacter::OnGameplayEffectApplied(UAbilitySystemComponent* Source, 
    const FGameplayEffectSpec& Spec, FActiveGameplayEffectHandle Handle)
{
    if (const UUEProjectGameplayEffect_ApplyDamage* DamageEffect = 
            Cast<UUEProjectGameplayEffect_ApplyDamage>(Spec.Def))
    {
        if (Spec.GetDynamicAssetTags().HasTag(TAG_Ability_LightAttack))
        {
            PlayAnimMontage(DamageEffect->GetHitReactionMontage());
        }
        else if (Spec.GetDynamicAssetTags().HasTag(TAG_Ability_HeavyAttack))
        {
            PlayAnimMontage(DamageEffect->GetHitReactionMontage());
        }
    }
}
