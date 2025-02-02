#pragma once

#include <CoreMinimal.h>
#include <GameFramework/GameModeBase.h>

#include "UEProjectGameMode.generated.h"


class AUEProjectSpawnPoint;

UCLASS()
class AUEProjectGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:

	//////////////////////////////////////////////////////////////////////////
    // AActor overrides

	virtual void BeginPlay() override;

	// AActor overrides
	//////////////////////////////////////////////////////////////////////////

	/** Spawns characters at all active spawn points in the world. */
	void SpawnCharacters();

private:

	/** Finds all active spawn points in the world. */
	TArray<AUEProjectSpawnPoint*> FindActiveSpawnPoints() const;
};
