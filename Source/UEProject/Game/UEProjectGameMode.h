// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/GameModeBase.h>

#include "UEProjectGameMode.generated.h"


class AUEProjectSpawnPoint;

UCLASS(minimalapi)
class AUEProjectGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	AUEProjectGameMode();

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
