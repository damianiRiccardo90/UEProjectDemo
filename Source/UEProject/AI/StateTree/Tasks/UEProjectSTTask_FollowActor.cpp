#include "UEProjectSTTask_FollowActor.h"

#include <AIController.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <Navigation/PathFollowingComponent.h>
#include <StateTreeExecutionContext.h>


const UStruct* FUEProjectSTTask_FollowActor::GetInstanceDataType() const
{
	return FInstanceDataType::StaticStruct();
}

#if WITH_EDITOR
FName FUEProjectSTTask_FollowActor::GetIconName() const
{
	return FName("StateTreeEditorStyle|Node.Navigation");
}
#endif // WITH_EDITOR

EStateTreeRunStatus FUEProjectSTTask_FollowActor::EnterState(
	FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	if (AAIController* const AIController = Cast<AAIController>(Context.GetOwner()))
	{
		if (const UWorld* const World = Context.GetWorld())
		{
			const EPathFollowingRequestResult::Type MoveResult = AIController->MoveToActor(
				InstanceData.TargetActor,
				InstanceData.AcceptableRadius,
				true, // Stop on overlap
				true, // Use pathfinding
				false // Cannot strafe
			);

			return MoveResult == EPathFollowingRequestResult::RequestSuccessful ? 
				EStateTreeRunStatus::Running : EStateTreeRunStatus::Failed;
		}
	}

	return EStateTreeRunStatus::Failed;
}

EStateTreeRunStatus FUEProjectSTTask_FollowActor::Tick(
	FStateTreeExecutionContext& Context, float DeltaTime) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	if (const AAIController* const AIController = Cast<AAIController>(Context.GetOwner()))
	{
		if (const UPathFollowingComponent* const PathFollowingComp = 
				AIController->GetPathFollowingComponent())
		{
			// If the NPC has stopped moving, the move is considered complete
			if (PathFollowingComp->GetStatus() != EPathFollowingStatus::Moving)
			{
				return EStateTreeRunStatus::Succeeded;
			}
		}
	}

	return EStateTreeRunStatus::Running;
}

void FUEProjectSTTask_FollowActor::ExitState(
	FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	// Stop movement if this state is exiting
	if (AAIController* const AIController = Cast<AAIController>(Context.GetOwner()))
	{
		AIController->StopMovement();
	}
}
