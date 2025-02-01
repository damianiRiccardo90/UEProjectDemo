#pragma once

#include <CoreMinimal.h>
#include <Tasks/StateTreeAITask.h>

#include "UEProjectSTTask_RunBehaviorTree.generated.h"


class AAIController;
class UBehaviorTree;
class UBlackboardComponent;
struct FInstancedPropertyBag;

USTRUCT()
struct FUEProjectSTInstanceData_RunBehaviorTree
{
	GENERATED_BODY()

	/** The AI controller that will run the tree. */
	UPROPERTY(EditAnywhere, Category = "Context")
	AAIController* AIController = nullptr;

	/** The behavior tree that will run. */
	UPROPERTY(EditAnywhere, Category = "Parameter")
	UBehaviorTree* BehaviorTree = nullptr;

	/** Input parameters that will end up in the blackboard. */
	UPROPERTY(EditAnywhere, Category = "Parameter")
	FInstancedPropertyBag InputParameters;

	/** If true the tree will loop, if false it will only run once. */
	UPROPERTY(EditAnywhere, Category = "Parameter")
	bool bLoop = true;
};

/**
 * This task runs an a behavior tree, supports looping and also input parameters 
 * that will be stored on the blackboard.
 */
USTRUCT(meta = (DisplayName = "Run Behavior Tree", Category = "AI|BehaviorTree"))
struct FUEProjectSTTask_RunBehaviorTree : public FStateTreeAITaskBase
{
	GENERATED_BODY()

	using FInstanceDataType = FUEProjectSTInstanceData_RunBehaviorTree;

public:

	//////////////////////////////////////////////////////////////////////////
	// FStateTreeNodeBase overrides

	virtual const UStruct* GetInstanceDataType() const override;

	// FStateTreeNodeBase overrides
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// FStateTreeTaskBase overrides

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, 
		const FStateTreeTransitionResult& Transition) const override;

	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, 
		const float DeltaTime) const override;

	virtual void ExitState(FStateTreeExecutionContext& Context,
		const FStateTreeTransitionResult& Transition) const override;

	// FStateTreeTaskBase overrides
	//////////////////////////////////////////////////////////////////////////

private:

	void SetBlackboardKeysFromPropertyBag(FStateTreeExecutionContext& Context, 
		UBlackboardComponent& BlackboardComponent) const;
};
