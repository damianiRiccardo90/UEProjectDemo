#include "UEProjectSTTask_RunBehaviorTree.h"

#include <BehaviorTree/BehaviorTree.h>
#include <BehaviorTree/BehaviorTreeComponent.h>
#include <BehaviorTree/BehaviorTreeTypes.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <StateTreeExecutionContext.h>

#include "UEProject/AI/Controller/UEProjectAIController.h"


const UStruct* FUEProjectSTTask_RunBehaviorTree::GetInstanceDataType() const
{
	return FInstanceDataType::StaticStruct();
}

EStateTreeRunStatus FUEProjectSTTask_RunBehaviorTree::EnterState(
	FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

    if (!InstanceData.AIController || !InstanceData.BehaviorTree)
    {
        return EStateTreeRunStatus::Failed;
    }
    
    UBlackboardComponent* BlackboardComponent = nullptr;
    if (!InstanceData.AIController->UseBlackboard(
            InstanceData.BehaviorTree->BlackboardAsset, BlackboardComponent))
    {
        return EStateTreeRunStatus::Failed;
    }

    SetBlackboardKeysFromPropertyBag(Context, *BlackboardComponent);

	InstanceData.AIController->RunBehaviorTreeWithLoop(InstanceData.BehaviorTree, InstanceData.bLoop);

	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FUEProjectSTTask_RunBehaviorTree::Tick(
    FStateTreeExecutionContext& Context, const float DeltaTime) const
{
    FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

    UBehaviorTreeComponent* const BehaviorTreeComponent = 
		Cast<UBehaviorTreeComponent>(InstanceData.AIController->GetBrainComponent());
	if (!BehaviorTreeComponent) return EStateTreeRunStatus::Failed;

    if (!BehaviorTreeComponent->IsRunning()) return EStateTreeRunStatus::Succeeded;

	return EStateTreeRunStatus::Running;
}

void FUEProjectSTTask_RunBehaviorTree::ExitState(FStateTreeExecutionContext& Context, 
    const FStateTreeTransitionResult& Transition) const
{
    FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

    UBehaviorTreeComponent* const BehaviorTreeComponent = 
		Cast<UBehaviorTreeComponent>(InstanceData.AIController->GetBrainComponent());
	if (!BehaviorTreeComponent) return;

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
    if (!PropertyBag) return;

    const int32 NumProperties = InstanceData.InputParameters.GetNumPropertiesInBag();
    for (int32 PropertyIdx = 0; PropertyIdx < NumProperties; PropertyIdx++)
    {
        const FPropertyBagPropertyDesc& PropDesc = PropertyBag->GetPropertyDescs()[PropertyIdx];

        const FName KeyName = PropDesc.Name;

        // Skip, if the blackboard does not have a matching key
        if (BlackboardComponent.GetKeyID(KeyName) == FBlackboard::InvalidKey) continue;

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
