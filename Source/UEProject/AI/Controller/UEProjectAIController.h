#pragma once

#include <AIController.h>
#include <CoreMinimal.h>

#include "UEProjectAIController.generated.h"


class UStateTreeAIComponent;
class UUEProjectAIPerceptionConfig;

/**
 * Base AI Controller that runs a State Tree.
 */
UCLASS(Blueprintable, Category = "AI", meta = (DisplayName = "Base AI Controller"))
class AUEProjectAIController : public AAIController
{
	GENERATED_BODY()

public:

	AUEProjectAIController(
		const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//////////////////////////////////////////////////////////////////////////
	// IGenericTeamAgentInterface overrides

	virtual FGenericTeamId GetGenericTeamId() const override;

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	// IGenericTeamAgentInterface overrides
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// AAIController overrides

	void PostInitializeComponents() override;

	// AAIController overrides
	//////////////////////////////////////////////////////////////////////////

	UStateTreeAIComponent* GetStateTreeAIComponent() const;

	bool RunBehaviorTreeWithLoop(UBehaviorTree* BTAsset, bool bLoop = true);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AIController")
	UStateTreeAIComponent* StateTreeAIComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AIController | Perception")
	UUEProjectAIPerceptionConfig* PerceptionConfig;
};