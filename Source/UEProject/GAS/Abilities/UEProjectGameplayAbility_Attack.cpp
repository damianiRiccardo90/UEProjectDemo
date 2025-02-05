#include "UEProjectGameplayAbility_Attack.h"

#include <Abilities/Tasks/AbilityTask_PlayMontageAndWait.h>
#include <AbilitySystemComponent.h>
#include <AbilitySystemGlobals.h>
#include <Components/SkeletalMeshComponent.h>
#include <GameFramework/Character.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>

#include "UEProject/GAS/UEProjectGameplayTagsLibrary.h"


UUEProjectGameplayAbility_Attack::UUEProjectGameplayAbility_Attack(
    const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , AttackMontage(nullptr)
    , AttackRange(100.f)
    , BaseAttackDamage(10.f)
    , LightAttackDamageMultiplier(2.f)
    , HeavyAttackDamageMultiplier(5.f)
    , DamageEffect(nullptr)
    , AttackType(FGameplayTag::EmptyTag)
{
    const FGameplayTagContainer AbilityTagsContainer = 
        FGameplayTagContainer::CreateFromArray(TArray<FGameplayTag>{ 
            TAG_Ability_LightAttack, TAG_Ability_HeavyAttack 
    });

    SetAssetTags(AbilityTagsContainer);
    BlockAbilitiesWithTag.AppendTags(AbilityTagsContainer);

    FAbilityTriggerData LightAttackTrigger;
    LightAttackTrigger.TriggerTag = TAG_Ability_LightAttack;
    LightAttackTrigger.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
    AbilityTriggers.Add(LightAttackTrigger);

    FAbilityTriggerData HeavyAttackTrigger;
    HeavyAttackTrigger.TriggerTag = TAG_Ability_HeavyAttack;
    HeavyAttackTrigger.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
    AbilityTriggers.Add(HeavyAttackTrigger);
}

bool UUEProjectGameplayAbility_Attack::CanActivateAbility( 
    const FGameplayAbilitySpecHandle Handle, 
    const FGameplayAbilityActorInfo* ActorInfo, 
    const FGameplayTagContainer* SourceTags, 
    const FGameplayTagContainer* TargetTags, 
    FGameplayTagContainer* OptionalRelevantTags ) const
{
    // Check if the base CanActivateAbility fails for any reason
    if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
    {
        return false;
    }

    // Make sure the montage and effect exist
    if (!AttackMontage || !DamageEffect)
    {
        return false;
    }

    return true;
}

void UUEProjectGameplayAbility_Attack::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData
)
{
    // Always check if we can activate
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        return;
    }

    // Cache the event activation tag
    if (TriggerEventData)
    {
        AttackType = TriggerEventData->EventTag;
    }

    // Play the attack montage
    if (AttackMontage)
    {
        // Create a montage task to asynchronously play the montage
        UAbilityTask_PlayMontageAndWait* const MontageTask = 
            UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
                this, NAME_None, AttackMontage, 1.0f, NAME_None, false, 1.0f);

        // Bind delegates
        MontageTask->OnCompleted.AddDynamic(this, &ThisClass::EndAbilityDirect);
        MontageTask->OnInterrupted.AddDynamic(this, &ThisClass::EndAbilityDirect);
        MontageTask->OnCancelled.AddDynamic(this, &ThisClass::EndAbilityDirect);

        MontageTask->ReadyForActivation();
    }
    else
    {
        // If the montage is invalid, just continue with checking damage
        EndAbilityDirect();
    }
}

TArray<FHitResult> UUEProjectGameplayAbility_Attack::PerformAttackTrace(
    const ACharacter* InstigatorCharacter) const
{
    // Find potential targets using a simple sphere trace

    TArray<FHitResult> OutHits;

    if (!InstigatorCharacter) return OutHits;

    const FVector Start = InstigatorCharacter->GetActorLocation();
    const FVector End = Start; // For a sphere, start and end are the same
    const float Radius = AttackRange;

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(InstigatorCharacter);

    GetWorld()->SweepMultiByChannel( OutHits, Start, End, FQuat::Identity, 
        ECC_Pawn, FCollisionShape::MakeSphere(Radius), QueryParams);

    return OutHits;
}

void UUEProjectGameplayAbility_Attack::ApplyAttackDamageToTargets(
    const TArray<FHitResult>& HitResults, const FGameplayAbilitySpec* CurrAbilitySpec)
{
    if (!CurrAbilitySpec) return;

    UAbilitySystemComponent* const ASC = GetAbilitySystemComponentFromActorInfo();
    if (!ASC) return;

    for (const FHitResult& Hit : HitResults)
    {
        const AActor* const TargetActor = Hit.GetActor();
        if (!TargetActor || TargetActor == GetAvatarActorFromActorInfo()) continue;

        FGameplayEffectSpecHandle SpecHandle = 
            ASC->MakeOutgoingSpec(DamageEffect, 1.0f, ASC->MakeEffectContext());
        if (SpecHandle.IsValid())
        {
            SpecHandle.Data->SetSetByCallerMagnitude(TAG_Data_Damage, 
                BaseAttackDamage * (AttackType == TAG_Ability_HeavyAttack ? 
                    HeavyAttackDamageMultiplier : LightAttackDamageMultiplier));

            SpecHandle.Data->AddDynamicAssetTag(AttackType);

            if (UAbilitySystemComponent* const TargetASC =
                    UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor))
            {
                TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
            }
        }

        // For single-target attack, just break if you only want the first valid
        break;
    }
}

void UUEProjectGameplayAbility_Attack::EndAbilityDirect()
{
    const ACharacter* const AvatarCharacter = Cast<ACharacter>(GetAvatarActorFromActorInfo());
    if (!AvatarCharacter)
    {
        EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), 
            true, false);
        return;
    }

    const FGameplayAbilitySpec* const CurrAbilitySpec = GetCurrentAbilitySpec();
    if (!CurrAbilitySpec)
    {
        EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), 
            true, false);
        return;
    }

    // Perform collision trace
    const TArray<FHitResult> HitResults = PerformAttackTrace(AvatarCharacter);

    // Compute the damage and apply it to victims
    ApplyAttackDamageToTargets(HitResults, CurrAbilitySpec);

    EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), 
        true, false);
}
