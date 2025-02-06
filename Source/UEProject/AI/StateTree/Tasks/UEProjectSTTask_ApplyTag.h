#pragma once

#include <CoreMinimal.h>
#include <GameplayTagContainer.h>
#include <Tasks/StateTreeAITask.h>

#include "UEProjectSTTask_ApplyTag.generated.h"


/** Instance data struct for ApplyTag task. */
USTRUCT()
struct FUEProjectSTInstanceData_ApplyTag
{
	GENERATED_BODY()

	/** The gameplay tag we want to set or remove. */
	UPROPERTY(EditAnywhere, Category = "Parameter")
	FGameplayTag GameplayTag = FGameplayTag::EmptyTag;

	/**
	 * If false, we add the tag on Enter and remove it on Exit.
	 * If true, we remove the tag on Enter and add it on Exit.
	 */
	UPROPERTY(EditAnywhere, Category = "Parameter", meta = (DisplayName = "Invert Behavior"))
	bool bInvertBehavior = false;
};

/**
 * This State Tree task adds (or removes) a gameplay tag to on state Enter, 
 * and then reverts that change on state Exit.
 */
USTRUCT(meta = (DisplayName = "Apply Tag", Category = "AI | GAS"))
struct FUEProjectSTTask_ApplyTag : public FStateTreeAITaskBase
{
	GENERATED_BODY()

	using FInstanceDataType = FUEProjectSTInstanceData_ApplyTag;

	//////////////////////////////////////////////////////////////////////////
	// FStateTreeNodeBase overrides

	virtual const UStruct* GetInstanceDataType() const override;

#if WITH_EDITOR
	virtual FText GetDescription(
		const FGuid& ID, 
		FStateTreeDataView InstanceDataView, 
		const IStateTreeBindingLookup& BindingLookup, 
		EStateTreeNodeFormatting Formatting = EStateTreeNodeFormatting::Text
	) const override;

	virtual FName GetIconName() const override;
#endif // WITH_EDITOR

	// FStateTreeNodeBase overrides
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// FStateTreeTaskBase overrides

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, 
		const FStateTreeTransitionResult& Transition) const override;

	virtual void ExitState(FStateTreeExecutionContext& Context,
		const FStateTreeTransitionResult& Transition) const override;

	// FStateTreeTaskBase overrides
	//////////////////////////////////////////////////////////////////////////
};
