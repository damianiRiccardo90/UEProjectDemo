#include "UEProjectSTCondition_IsActorPerceived.h"

#include <AIController.h>
#include <GameFramework/Actor.h>
#include <Perception/AIPerceptionComponent.h>
#include <StateTreeExecutionContext.h>


const UStruct* FUEProjectSTCondition_IsActorPerceived::GetInstanceDataType() const
{
    return FInstanceDataType::StaticStruct();
}

bool FUEProjectSTCondition_IsActorPerceived::TestCondition(FStateTreeExecutionContext& Context) const
{
    FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

    AAIController* const AIController = Cast<AAIController>(Context.GetOwner());
    if (!AIController) return false;

    UAIPerceptionComponent* const PerceptionComponent = AIController->GetPerceptionComponent();
    if (!PerceptionComponent) return false;

    // Accumulate perceived actors
    TArray<AActor*> PerceivedActors;
    if (InstanceData.bCheckAllSenses || InstanceData.SensesToCheck.IsEmpty())
    {
        // Check all senses
        PerceptionComponent->GetCurrentlyPerceivedActors(nullptr, PerceivedActors);
    }
    else
    {
        // Only check specific sense classes
        for (const TSubclassOf<UAISense>& SenseClass : InstanceData.SensesToCheck)
        {
            if (!SenseClass) continue;

            TArray<AActor*> PerceivedBySense;
            PerceptionComponent->GetCurrentlyPerceivedActors(SenseClass, PerceivedBySense);
            // Append results to our main list
            PerceivedActors.Append(PerceivedBySense);
        }
    }

    const UWorld* const World = Context.GetWorld();
    if (!World) return false;

    // Check if any stimuli are valid in the last AllowedMemoryTime
    for (AActor* const PerceivedActor : PerceivedActors)
    {
        if (!PerceivedActor || PerceivedActor != InstanceData.TargetActor) continue;

        FActorPerceptionBlueprintInfo Info;
        PerceptionComponent->GetActorsPerception(PerceivedActor, Info);

        for (const FAIStimulus& Stimulus : Info.LastSensedStimuli)
        {
            if (Stimulus.IsValid())
            {
                // Condition is if timeout is not breached
                return World->GetTimeSeconds() - Stimulus.GetAge() <= InstanceData.SenseTimeout;
            }
        }
    }

    // No valid stimuli within memory time
    return false;
}
