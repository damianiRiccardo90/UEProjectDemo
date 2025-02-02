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
    , AttackRange(200.f)
    , AttackDamage(25.f)
    , DamageEffect(nullptr)
{}

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

    // Play the attack montage
    if (AttackMontage && ActorInfo->AnimInstance.IsValid())
    {
        // Create a montage task to asynchronously play the montage
        UAbilityTask_PlayMontageAndWait* const MontageTask = 
            UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
                this, NAME_None, AttackMontage, 1.0f, NAME_None, false, 1.0f);

        // Bind delegates
        MontageTask->OnCompleted.AddDynamic(this, &UUEProjectGameplayAbility_Attack::EndAbilityDirect);
        MontageTask->OnInterrupted.AddDynamic(this, &UUEProjectGameplayAbility_Attack::EndAbilityDirect);
        MontageTask->OnCancelled.AddDynamic(this, &UUEProjectGameplayAbility_Attack::EndAbilityDirect);

        MontageTask->ReadyForActivation();
    }
    else
    {
        // If the montage is invalid, just continue with checking damage
        EndAbilityDirect();
    }
}

void UUEProjectGameplayAbility_Attack::EndAbilityDirect()
{
    // Attempt to find a valid target within range
    ACharacter* const AvatarCharacter = Cast<ACharacter>(GetAvatarActorFromActorInfo());
    if (!AvatarCharacter)
    {
        EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), 
            true, false);
        return;
    }

    // Find potential targets using a simple sphere trace
    TArray<FHitResult> HitResults;
    const FVector Start = AvatarCharacter->GetActorLocation();
    const FVector End = Start; // For a sphere, start and end are the same
    const float Radius = AttackRange;

    // Sphere trace for Pawn channel
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(AvatarCharacter);

    const bool bHit = GetWorld()->SweepMultiByChannel(HitResults, Start, End, 
        FQuat::Identity, ECC_Pawn, FCollisionShape::MakeSphere(Radius), QueryParams);

    if (bHit)
    {
        for (FHitResult& Hit : HitResults)
        {
            AActor* const TargetActor = Hit.GetActor();
            if (TargetActor && TargetActor != AvatarCharacter)
            {
                if (UAbilitySystemComponent* const ASC = GetAbilitySystemComponentFromActorInfo())
                {
                    // Build the effect spec from our effect class
                    FGameplayEffectSpecHandle SpecHandle = 
                        ASC->MakeOutgoingSpec(DamageEffect, 1.0f, ASC->MakeEffectContext());
                    if (SpecHandle.IsValid())
                    {
                        SpecHandle.Data->SetSetByCallerMagnitude(TAG_Data_Damage, AttackDamage);

                        if (UAbilitySystemComponent* const TargetASC =
                                UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor))
                        {
                            ASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
                        }
                    }
                }

                // For a single-target attack, break after the first valid target
                break;
            }
        }
    }

    EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), 
        true, false);
}
