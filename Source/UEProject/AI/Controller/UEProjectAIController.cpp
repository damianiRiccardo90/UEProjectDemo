#include "UEProjectAIController.h"

#include <BehaviorTree/BehaviorTree.h>
#include <BehaviorTree/BehaviorTreeComponent.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <Components/StateTreeAIComponent.h>


AUEProjectAIController::AUEProjectAIController()
	: StateTreeAIComponent(nullptr)
{
	StateTreeAIComponent = CreateDefaultSubobject<UStateTreeAIComponent>("StateTreeAIComponent");
}

UStateTreeAIComponent* AUEProjectAIController::GetStateTreeAIComponent() const
{
	return StateTreeAIComponent;
}

bool AUEProjectAIController::RunBehaviorTreeWithLoop(UBehaviorTree* BTAsset, bool bLoop /*= true*/)
{
	if (!BTAsset) return false;

	bool bSuccess = true;

	// See if we need a blackboard component at all
	UBlackboardComponent* BlackboardComponent = Blackboard;
	if (BTAsset->BlackboardAsset && 
		(!Blackboard || !Blackboard->IsCompatibleWith(BTAsset->BlackboardAsset)))
	{
		bSuccess = UseBlackboard(BTAsset->BlackboardAsset, BlackboardComponent);
	}

	if (bSuccess)
	{
		UBehaviorTreeComponent* BehaviorTreeComponent = 
			Cast<UBehaviorTreeComponent>(BrainComponent);
		if (!BehaviorTreeComponent)
		{
			BehaviorTreeComponent = NewObject<UBehaviorTreeComponent>(
				this, TEXT("BehaviorTreeComponent"));
			BehaviorTreeComponent->RegisterComponent();
		}
		
		// Make sure BrainComponent points at the newly created BehaviorTreeComponent
		BrainComponent = BehaviorTreeComponent;

		if (BehaviorTreeComponent)
		{
			BehaviorTreeComponent->StartTree(*BTAsset,
				bLoop ? EBTExecutionMode::Looped : EBTExecutionMode::SingleRun);
		}
		else bSuccess = false;
	}

	return bSuccess;
}
