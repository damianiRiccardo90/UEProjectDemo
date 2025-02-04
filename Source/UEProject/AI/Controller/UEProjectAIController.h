#pragma once

#include <AIController.h>
#include <CoreMinimal.h>

#include "UEProjectAIController.generated.h"


class UStateTreeAIComponent;

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

	UStateTreeAIComponent* GetStateTreeAIComponent() const;

	bool RunBehaviorTreeWithLoop(UBehaviorTree* BTAsset, bool bLoop = true);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AIController")
	UStateTreeAIComponent* StateTreeAIComponent;
};