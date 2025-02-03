#include "UEProjectBTTask_MoveToNextPatrolPoint.h"

#include <AIController.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <Navigation/PathFollowingComponent.h>

#include "UEProject/AI/Patrol/UEProjectPatrolNetwork.h"


UUEProjectBTTask_MoveToNextPatrolPoint::UUEProjectBTTask_MoveToNextPatrolPoint(
    const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , AcceptableRadius(50.f)
{
    NodeName = TEXT("Move To Next Patrol Point");
    bNotifyTick = true;
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

void UUEProjectBTTask_MoveToNextPatrolPoint::TickTask(UBehaviorTreeComponent& OwnerComp, 
    uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    const AAIController* const AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    const UPathFollowingComponent* const PathFollowingComponent = 
        AIController->GetPathFollowingComponent();
    if (!PathFollowingComponent)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    // Check PathFollowing status
    if (PathFollowingComponent->GetStatus() != EPathFollowingStatus::Moving)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
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
