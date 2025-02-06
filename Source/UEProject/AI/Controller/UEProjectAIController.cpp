#include "UEProjectAIController.h"

#include <BehaviorTree/BehaviorTree.h>
#include <BehaviorTree/BehaviorTreeComponent.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <Components/StateTreeAIComponent.h>
#include <Perception/AIPerceptionComponent.h>
#include <Perception/AISenseConfig_Sight.h>
#include <Perception/AISenseConfig_Touch.h>

#include "UEProject/AI/Perception/UEProjectAIPerceptionConfig.h"


AUEProjectAIController::AUEProjectAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, StateTreeAIComponent(nullptr)
	, PerceptionConfig(nullptr)
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));

	StateTreeAIComponent = CreateDefaultSubobject<UStateTreeAIComponent>("StateTreeAIComponent");
}

void AUEProjectAIController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (UAIPerceptionComponent* const PerceptionComp = GetAIPerceptionComponent())
	{
		if (PerceptionConfig)
		{
			// Configure all sense configs from the data asset
			for (UAISenseConfig* SenseConfig : PerceptionConfig->SenseConfigs)
			{
				if (SenseConfig)
				{
					PerceptionComp->ConfigureSense(*SenseConfig);
				}
			}

			// Set dominant sense if specified
			if (PerceptionConfig->DominantSenseClass)
			{
				PerceptionComp->SetDominantSense(PerceptionConfig->DominantSenseClass);
			}
		}
	}
}

FGenericTeamId AUEProjectAIController::GetGenericTeamId() const
{
	return 1;
}

ETeamAttitude::Type AUEProjectAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const IGenericTeamAgentInterface* const OtherTeamAgent = 
		Cast<IGenericTeamAgentInterface>(&Other);
	if (!OtherTeamAgent) return ETeamAttitude::Neutral;

	const uint8 OtherAgentTeamID = OtherTeamAgent->GetGenericTeamId().GetId();

	if (OtherAgentTeamID == 1) return ETeamAttitude::Friendly;
	if (OtherAgentTeamID == 0) return ETeamAttitude::Hostile;

	return ETeamAttitude::Neutral;
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
