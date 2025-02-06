#pragma once

#include <CoreMinimal.h>
#include <Tasks/StateTreeAITask.h>

#include "UEProjectSTTask_FollowActor.generated.h"


/**
 * Holds instance data for the Follow Actor StateTree task.
 */
USTRUCT()
struct FUEProjectSTInstanceData_FollowActor
{
	GENERATED_BODY()

	/** The target actor to follow. */
	UPROPERTY(EditAnywhere, Category = "Parameter")
	AActor* TargetActor = nullptr;

	/** Acceptable distance to consider the follow destination reached. */
	UPROPERTY(EditAnywhere, Category = "Parameter")
	float AcceptableRadius = 100.f;
};

/**
 * A StateTree task that instructs an AI-controlled NPC to follow a 
 * specified target actor.
 */
USTRUCT(meta = (DisplayName = "Follow Actor", Category = "AI | Movement"))
struct FUEProjectSTTask_FollowActor : public FStateTreeAITaskBase
{
	GENERATED_BODY()

	using FInstanceDataType = FUEProjectSTInstanceData_FollowActor;

	//////////////////////////////////////////////////////////////////////////
	// FStateTreeNodeBase overrides

	virtual const UStruct* GetInstanceDataType() const override;

#if WITH_EDITOR
	virtual FName GetIconName() const override;
#endif // WITH_EDITOR

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
};
