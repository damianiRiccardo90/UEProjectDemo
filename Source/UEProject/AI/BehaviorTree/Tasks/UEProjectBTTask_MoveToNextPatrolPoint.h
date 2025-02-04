#pragma once

#include <CoreMinimal.h>
#include <BehaviorTree/BTTaskNode.h>

#include "UEProjectBTTask_MoveToNextPatrolPoint.generated.h"


UCLASS()
class UUEProjectBTTask_MoveToNextPatrolPoint : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UUEProjectBTTask_MoveToNextPatrolPoint(
		const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//////////////////////////////////////////////////////////////////////////
	// UBTTaskNode overrides

	virtual EBTNodeResult::Type ExecuteTask(
		UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, 
		uint8* NodeMemory, float DeltaSeconds) override;

	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

	// UBTTaskNode overrides
	//////////////////////////////////////////////////////////////////////////

protected:

	/** Blackboard key for patrol network reference. */
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector PatrolNetworkKey;

    /** Blackboard key to store current patrol point index. */
    UPROPERTY(EditAnywhere, Category = "Blackboard") 
    FBlackboardKeySelector CurrentIndexKey;

    /** Acceptable radius to consider point reached. */
    UPROPERTY(EditAnywhere, Category = "Movement")
    float AcceptableRadius;
};
