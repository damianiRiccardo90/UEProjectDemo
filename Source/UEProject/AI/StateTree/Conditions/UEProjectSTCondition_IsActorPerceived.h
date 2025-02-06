#pragma once

#include <Conditions/StateTreeAIConditionBase.h>
#include <CoreMinimal.h>
#include <Perception/AISense.h>

#include "UEProjectSTCondition_IsActorPerceived.generated.h"


class AController;
class UAIPerceptionComponent;

USTRUCT()
struct FUEProjectSTInstanceData_IsActorPerceived
{
    GENERATED_BODY()

    /** The target actor to follow. */
	UPROPERTY(EditAnywhere, Category = "Input")
	AActor* TargetActor = nullptr;

    /**
     * How long (in seconds) after being perceived do we still consider the stimuli valid?
     * If Stimulus is older than this threshold, the condition will fail.
     */
    UPROPERTY(EditAnywhere, Category = "Parameter")
    float SenseTimeout = 2.f;

    /** Whether to check all senses or only specific ones. */
    UPROPERTY(EditAnywhere, Category = "Parameter", meta = (DisplayName = "Check All Senses"))
    bool bCheckAllSenses = true;

    /** Lists which AI Sense classes you want to check. */
    UPROPERTY(EditAnywhere, Category = "Parameter", meta=(EditCondition="!bCheckAllSenses"))
    TArray<TSubclassOf<UAISense>> SensesToCheck;
};

/**
 * A State Tree condition node that checks if the AI has perceived a target actor (e.g. a player).
 */
USTRUCT(meta = (DisplayName = "Is Actor Perceived", Category = "AI | Perception"))
struct FUEProjectSTCondition_IsActorPerceived : public FStateTreeAIConditionBase
{
    GENERATED_BODY()

    using FInstanceDataType = FUEProjectSTInstanceData_IsActorPerceived;

    //////////////////////////////////////////////////////////////////////////
    // FStateTreeNodeBase overrides

    virtual const UStruct* GetInstanceDataType() const override;

    // FStateTreeNodeBase overrides
    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    // FStateTreeConditionBase overrides

    virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;

    // FStateTreeConditionBase overrides
    //////////////////////////////////////////////////////////////////////////
};