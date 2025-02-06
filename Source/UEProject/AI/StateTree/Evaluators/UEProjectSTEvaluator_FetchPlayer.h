#pragma once

#include <CoreMinimal.h>
#include <StateTreeEvaluatorBase.h>
#include <StateTreeExecutionContext.h>

#include "UEProjectSTEvaluator_FetchPlayer.generated.h"


/** Instance data struct for FetchPlayer task. */
USTRUCT()
struct FUEProjectSTInstanceData_FetchPlayer
{
	GENERATED_BODY()

	/** The index of the target player to fetch. */
	UPROPERTY(EditAnywhere, Category = "Parameter")
	int32 PlayerIndex = 0;

	/** The target player reference to bind to. */
	UPROPERTY(EditAnywhere, Category = "Output")
	AActor* PlayerReference = nullptr;
};

/**
 * Evaluator the exposes a reference to any player actor for other node parameters to bind to.
 */
USTRUCT(meta = (DisplayName = "Fetch Player", Category = "Common"))
struct FUEProjectSTEvaluator_FetchPlayer : public FStateTreeEvaluatorCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FUEProjectSTInstanceData_FetchPlayer;

	//////////////////////////////////////////////////////////////////////////
	// FStateTreeNodeBase overrides

	virtual const UStruct* GetInstanceDataType() const override;

	// FStateTreeNodeBase overrides
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// FStateTreeEvaluatorBase overrides

	virtual void TreeStart(FStateTreeExecutionContext& Context) const override;

	// FStateTreeEvaluatorBase overrides
	//////////////////////////////////////////////////////////////////////////
};
