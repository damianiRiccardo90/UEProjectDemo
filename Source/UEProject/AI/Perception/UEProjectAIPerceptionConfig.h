#pragma once

#include <CoreMinimal.h>
#include <Engine/DataAsset.h>
#include <Perception/AISenseConfig.h>

#include "UEProjectAIPerceptionConfig.generated.h"


/**
 * List of AI Sense configurations to apply. 
 */
UCLASS(BlueprintType)
class UUEProjectAIPerceptionConfig : public UDataAsset
{
    GENERATED_BODY()

public:

    /** AI Sense configurations. */
    UPROPERTY(EditDefaultsOnly, Instanced, Category = "Perception")
    TArray<UAISenseConfig*> SenseConfigs;

    /** Optional: Specify which sense type should be dominant. */
    UPROPERTY(EditDefaultsOnly, Category = "Perception")
    TSubclassOf<UAISense> DominantSenseClass;
};
