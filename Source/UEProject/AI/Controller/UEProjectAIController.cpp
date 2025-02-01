#include "UEProjectAIController.h"

#include <Components/StateTreeComponent.h>


AUEProjectAIController::AUEProjectAIController()
	: StateTreeComponent(nullptr)
{
	StateTreeComponent = CreateDefaultSubobject<UStateTreeComponent>("StateTreeComponent");
}
