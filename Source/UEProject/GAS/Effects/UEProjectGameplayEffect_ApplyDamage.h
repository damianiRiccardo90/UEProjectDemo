#pragma once

#include <CoreMinimal.h>
#include <GameplayEffect.h>

#include "UEProjectGameplayEffect_ApplyDamage.generated.h"


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
};
