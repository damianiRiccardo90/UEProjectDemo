#include "UEProjectBTTask_MoveToNextPatrolPoint.h"

#include <AIController.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <Navigation/PathFollowingComponent.h>

#include "UEProject/AI/Patrol/UEProjectPatrolNetwork.h"


UUEProjectBTTask_MoveToNextPatrolPoint::UUEProjectBTTask_MoveToNextPatrolPoint()
    : AcceptableRadius(50.f)
{
    NodeName = TEXT("Move To Next Patrol Point");
}

EBTNodeResult::Type UUEProjectBTTask_MoveToNextPatrolPoint::ExecuteTask(
    UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* const AIController = OwnerComp.GetAIOwner();
    UBlackboardComponent* const Blackboard = OwnerComp.GetBlackboardComponent();
    
    if (!AIController || !Blackboard) return EBTNodeResult::Failed;

    AUEProjectPatrolNetwork* const PatrolNetwork = Cast<AUEProjectPatrolNetwork>(
        Blackboard->GetValueAsObject(PatrolNetworkKey.SelectedKeyName));    
    if (!PatrolNetwork) return EBTNodeResult::Failed;

    // Get current index
    int32 CurrentIndex = Blackboard->GetValueAsInt(CurrentIndexKey.SelectedKeyName);

    // If this is the first time (CurrentIndex is invalid), find nearest point
    if (!PatrolNetwork->IsValidPointIndex(CurrentIndex))
    {
        float Distance;
        APawn* const ControlledPawn = AIController->GetPawn();
        if (ControlledPawn && PatrolNetwork->FindNearestPointOnPath(
                ControlledPawn->GetActorLocation(), Distance, CurrentIndex))
        {
            Blackboard->SetValueAsInt(CurrentIndexKey.SelectedKeyName, CurrentIndex);
        }
        else return EBTNodeResult::Failed;
    }
    
    // Get next patrol point
    FVector NextLocation;
    if (!PatrolNetwork->GetNextPatrolPoint(CurrentIndex, NextLocation))
    {
        return EBTNodeResult::Failed;
    }

    // Update current index in blackboard
    Blackboard->SetValueAsInt(CurrentIndexKey.SelectedKeyName, 
        PatrolNetwork->GetNextPointIndex(CurrentIndex));

    // Move to point
    const EPathFollowingRequestResult::Type MoveResult = AIController->MoveToLocation(
        NextLocation,
        AcceptableRadius,
        true, // Stop on overlap
        true, // Use pathfinding
        true, // Project destination
        false // Cannot strafe
    );

    return MoveResult == EPathFollowingRequestResult::Type::RequestSuccessful ? 
        EBTNodeResult::InProgress : EBTNodeResult::Failed;
}

void UUEProjectBTTask_MoveToNextPatrolPoint::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, 
    uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
    if (AAIController* const AIController = OwnerComp.GetAIOwner())
    {
        AIController->StopMovement();
    }
    
    Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}

void UUEProjectBTTask_MoveToNextPatrolPoint::OnMessage(UBehaviorTreeComponent& OwnerComp, 
    uint8* NodeMemory, FName Message, int32 SenderID, bool bSuccess)
{
    if (Message == UBrainComponent::AIMessage_MoveFinished)
    {
        FinishLatentTask(OwnerComp, 
            bSuccess ? EBTNodeResult::Succeeded : EBTNodeResult::Failed);
    }
}
