#pragma once

#include <CoreMinimal.h>
#include <Engine/DataAsset.h>
#include <GameplayAbilitySpec.h>

#include "UEProjectGameplayAbilitySet.generated.h"


class UGameplayAbility;

/**
 * Describes a single ability to grant, can be extended in the future with 
 * things like ability level and such.
 */
USTRUCT(BlueprintType)
struct FGrantAbility
{
    GENERATED_BODY()

public:

    /** The Gameplay Ability class to grant. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<UGameplayAbility> AbilityClass;
};

/**
 * A data asset that contains a list of abilities to grant to a character,
 * which can be overridden by child classes or new assets.
 */
UCLASS(BlueprintType)
class UUEProjectGameplayAbilitySet : public UDataAsset
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FGrantAbility> GrantedAbilities;
};
