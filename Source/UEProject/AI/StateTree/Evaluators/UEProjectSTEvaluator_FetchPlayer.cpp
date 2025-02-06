#include "UEProjectSTEvaluator_FetchPlayer.h"

#include <Kismet/GameplayStatics.h>


const UStruct* FUEProjectSTEvaluator_FetchPlayer::GetInstanceDataType() const
{
	return FInstanceDataType::StaticStruct();
}

void FUEProjectSTEvaluator_FetchPlayer::TreeStart(FStateTreeExecutionContext& Context) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	if (const UWorld* const World = Context.GetWorld())
	{
		if (AActor* const PlayerActor = 
				UGameplayStatics::GetPlayerPawn(World, InstanceData.PlayerIndex))
		{
			InstanceData.PlayerReference = PlayerActor;
		}
	}
}
