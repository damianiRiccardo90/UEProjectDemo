#include "UEProjectSTTask_RunBehaviorTree.h"

#include <AIController.h>
#include <BehaviorTree/BehaviorTreeComponent.h>
#include <BehaviorTree/BehaviorTreeTypes.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <StateTreeExecutionContext.h>
#include <VisualLogger/VisualLogger.h>


const UStruct* FUEProjectSTTask_RunBehaviorTree::GetInstanceDataType() const
{
	return FInstanceDataType::StaticStruct();
}

EStateTreeRunStatus FUEProjectSTTask_RunBehaviorTree::EnterState(
	FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	if (!InstanceData.AIController)
	{
		UE_VLOG(Context.GetOwner(), LogStateTree, Error, 
			TEXT("AIController parameter is missing."));
		return EStateTreeRunStatus::Failed;
	}
	if (!InstanceData.BehaviorTree)
	{
		UE_VLOG(Context.GetOwner(), LogStateTree, Error, 
			TEXT("BehaviorTree parameter is missing."));
		return EStateTreeRunStatus::Failed;
	}

	UBehaviorTreeComponent* const BehaviorTreeComponent = 
		Cast<UBehaviorTreeComponent>(InstanceData.AIController->GetBrainComponent());
	if (!BehaviorTreeComponent)
	{
		UE_VLOG(Context.GetOwner(), LogStateTree, Error, 
			TEXT("Unable to retrieve BehaviorTreeComponent from AIController."));
		return EStateTreeRunStatus::Failed;
	}

	UBlackboardComponent* const BlackboardComponent = 
		BehaviorTreeComponent->GetBlackboardComponent();
    if (BlackboardComponent)
    {
        SetBlackboardKeysFromPropertyBag(Context, *BlackboardComponent);
    }

	BehaviorTreeComponent->StartTree(*InstanceData.BehaviorTree, 
		InstanceData.bLoop ? EBTExecutionMode::Looped : EBTExecutionMode::SingleRun);

	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FUEProjectSTTask_RunBehaviorTree::Tick(
    FStateTreeExecutionContext& Context, const float DeltaTime) const
{
    FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

    UBehaviorTreeComponent* const BehaviorTreeComponent = 
		Cast<UBehaviorTreeComponent>(InstanceData.AIController->GetBrainComponent());
	if (!BehaviorTreeComponent)
	{
		UE_VLOG(Context.GetOwner(), LogStateTree, Error, 
			TEXT("Unable to retrieve BehaviorTreeComponent from AIController."));
		return EStateTreeRunStatus::Failed;
	}

    if (!BehaviorTreeComponent->IsRunning())
    {
        return EStateTreeRunStatus::Succeeded;
    }

	return EStateTreeRunStatus::Running;
}

void FUEProjectSTTask_RunBehaviorTree::ExitState(FStateTreeExecutionContext& Context, 
    const FStateTreeTransitionResult& Transition) const
{
    FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

    UBehaviorTreeComponent* const BehaviorTreeComponent = 
		Cast<UBehaviorTreeComponent>(InstanceData.AIController->GetBrainComponent());
	if (!BehaviorTreeComponent)
	{
		UE_VLOG(Context.GetOwner(), LogStateTree, Error, 
			TEXT("Unable to retrieve BehaviorTreeComponent from AIController."));
        return;
	}

    if (BehaviorTreeComponent->IsRunning())
    {
        BehaviorTreeComponent->StopLogic(
            TEXT("RunBehaviorTree owner state has finished running."));
    }
}

void FUEProjectSTTask_RunBehaviorTree::SetBlackboardKeysFromPropertyBag(
    FStateTreeExecutionContext& Context, UBlackboardComponent& BlackboardComponent) const
{
    FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

    const UPropertyBag* const PropertyBag = InstanceData.InputParameters.GetPropertyBagStruct();
    if (!PropertyBag)
    {
        UE_VLOG(Context.GetOwner(), LogStateTree, Verbose, 
            TEXT("Unable to extract UPropertyBag ptr from FInstancedPropertyBag."));
        return;
    }

    const int32 NumProperties = InstanceData.InputParameters.GetNumPropertiesInBag();
    for (int32 PropertyIdx = 0; PropertyIdx < NumProperties; PropertyIdx++)
    {
        const FPropertyBagPropertyDesc& PropDesc = PropertyBag->GetPropertyDescs()[PropertyIdx];

        const FName KeyName = PropDesc.Name;

        // Skip, if the blackboard does not have a matching key
        if (BlackboardComponent.GetKeyID(KeyName) == FBlackboard::InvalidKey)
        {
            UE_VLOG(Context.GetOwner(), LogStateTree, Warning,
                TEXT("No matching Blackboard key named '%s'."), *KeyName.ToString());
            continue;
        }

        // Read the actual value from the property bag, and set it onto the BB key value
        switch (PropDesc.ValueType)
        {
            case EPropertyBagPropertyType::Bool:
            {
                BlackboardComponent.SetValueAsBool(KeyName,
                    InstanceData.InputParameters.GetValueBool(PropDesc.Name).GetValue());
                break;
            }
            case EPropertyBagPropertyType::Int32:
            {
                BlackboardComponent.SetValueAsInt(KeyName,
                    InstanceData.InputParameters.GetValueInt32(PropDesc.Name).GetValue());
                break;
            }
            case EPropertyBagPropertyType::Float:
            {
                BlackboardComponent.SetValueAsFloat(KeyName, 
                    InstanceData.InputParameters.GetValueFloat(PropDesc.Name).GetValue());
                break;
            }
            case EPropertyBagPropertyType::Name:
            {
                BlackboardComponent.SetValueAsName(KeyName, 
                    InstanceData.InputParameters.GetValueName(PropDesc.Name).GetValue());
                break;
            }
            case EPropertyBagPropertyType::String:
            {
                BlackboardComponent.SetValueAsString(KeyName, 
                    InstanceData.InputParameters.GetValueString(PropDesc.Name).GetValue());
                break;
            }
            case EPropertyBagPropertyType::Object:
            {
                BlackboardComponent.SetValueAsObject(KeyName,
                    InstanceData.InputParameters.GetValueObject(PropDesc.Name).GetValue());
                break;
            }
            case EPropertyBagPropertyType::Class:
            {
                BlackboardComponent.SetValueAsClass(KeyName,
                    InstanceData.InputParameters.GetValueClass(PropDesc.Name).GetValue());
                break;
            }
            default:
            {
                UE_VLOG(BlackboardComponent.GetOwner(), LogStateTree, Warning,
                    TEXT("Unsupported property type (%d) for key '%s'."),
                    (int32)PropDesc.ValueType, *KeyName.ToString());
                break;
            }
        }
    }
}
