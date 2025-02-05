#pragma once

#include <CoreMinimal.h>
#include <GameplayEffect.h>

#include "UEProjectGameplayEffect_ApplyDamage.generated.h"


class UAnimMontage;

/**
 * Gameplay Effect that applies instant damage. 
 * It uses a by-caller magnitude to handle the incoming damage amount.
 */
UCLASS()
class UUEProjectGameplayEffect_ApplyDamage : public UGameplayEffect
{
    GENERATED_BODY()

public:

    UUEProjectGameplayEffect_ApplyDamage(
        const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    UAnimMontage* GetHitReactionMontage() const;

protected:

    /** Animation montage to play when reacting to the damage being applied. */
    UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effect")
    UAnimMontage* HitReactionMontage;
};
