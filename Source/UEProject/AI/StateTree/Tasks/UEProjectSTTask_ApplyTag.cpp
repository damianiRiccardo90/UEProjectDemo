#include "UEProjectSTTask_ApplyTag.h"

#include <AbilitySystemComponent.h>
#include <AIController.h>
#include <StateTreeExecutionContext.h>

#include "UEProject/Characters/UEProjectBaseCharacter.h"


const UStruct* FUEProjectSTTask_ApplyTag::GetInstanceDataType() const
{
	return FInstanceDataType::StaticStruct();
}

#if WITH_EDITOR
FText FUEProjectSTTask_ApplyTag::GetDescription(
	const FGuid& ID, 
	FStateTreeDataView InstanceDataView, 
	const IStateTreeBindingLookup& BindingLookup, 
	EStateTreeNodeFormatting Formatting /*= EStateTreeNodeFormatting::Text */
) const
{
	const FInstanceDataType* const InstanceData = 
		InstanceDataView.GetPtr<FInstanceDataType>();
	check(InstanceData);

	const FStateTreePropertyPath TagPropertyPath = FStateTreePropertyPath(
		ID, GET_MEMBER_NAME_CHECKED(FInstanceDataType, GameplayTag));

	FText GameplayTagValue = 
		BindingLookup.GetBindingSourceDisplayName(TagPropertyPath, Formatting);

	if (GameplayTagValue.IsEmpty())
	{
		GameplayTagValue = FText::FromString(InstanceData->GameplayTag.ToString());
	}

	if (Formatting == EStateTreeNodeFormatting::RichText)
	{
		const FText Format = !InstanceData->bInvertBehavior ? 
			NSLOCTEXT("StateTree", "ApplyTagRichText", "<b>Apply Tag:</> {0}") :
			NSLOCTEXT("StateTree", "RemoveTagRichText", "<b>Remove Tag:</> {0}");
		return FText::Format(Format, GameplayTagValue);
	}

	return FText::Format(!InstanceData->bInvertBehavior ?
		NSLOCTEXT("StateTree", "ApplyTagText", "Apply Tag: {0}") :
		NSLOCTEXT("StateTree", "RemoveTagText", "Remove Tag: {0}"), 
		GameplayTagValue);
}

FName FUEProjectSTTask_ApplyTag::GetIconName() const
{
	return FName("StateTreeEditorStyle|Node.Tag");
}
#endif // WITH_EDITOR

EStateTreeRunStatus FUEProjectSTTask_ApplyTag::EnterState(
	FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	if (AAIController* const AIController = Cast<AAIController>(Context.GetOwner()))
	{
		if (AUEProjectBaseCharacter* const Character = 
				Cast<AUEProjectBaseCharacter>(AIController->GetCharacter()))
		{
			if (UAbilitySystemComponent* const ASC = Character->GetAbilitySystemComponent())
			{
				if (!InstanceData.bInvertBehavior)
				{
					ASC->AddLooseGameplayTag(InstanceData.GameplayTag);
				}
				else
				{
					ASC->RemoveLooseGameplayTag(InstanceData.GameplayTag);
				}
			}
		}
	}

	// Return Running so the StateTree knows we're still active until a transition occurs.
	return EStateTreeRunStatus::Running;
}

void FUEProjectSTTask_ApplyTag::ExitState(
	FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	if (AAIController* const AIController = Cast<AAIController>(Context.GetOwner()))
	{
		if (AUEProjectBaseCharacter* const Character = 
			Cast<AUEProjectBaseCharacter>(AIController->GetCharacter()))
		{
			if (UAbilitySystemComponent* const ASC = Character->GetAbilitySystemComponent())
			{
				if (!InstanceData.bInvertBehavior)
				{
					ASC->RemoveLooseGameplayTag(InstanceData.GameplayTag);
				}
				else
				{
					ASC->AddLooseGameplayTag(InstanceData.GameplayTag);
				}
			}
		}
	}
}
