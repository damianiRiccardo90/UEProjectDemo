#pragma once

#include <AIController.h>
#include <CoreMinimal.h>

#include "UEProjectAIController.generated.h"


class UStateTreeComponent;

/**
 * Base AI Controller that runs a State Tree.
 */
UCLASS(Blueprintable, Category = "AI", meta = (DisplayName = "Base AI Controller"))
class AUEProjectAIController : public AAIController
{
	GENERATED_BODY()

public:

	AUEProjectAIController();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AIController")
	UStateTreeComponent* StateTreeComponent;
};