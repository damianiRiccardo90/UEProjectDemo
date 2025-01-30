// Copyright Epic Games, Inc. All Rights Reserved.

#include "UEProjectGameMode.h"

#include <EngineUtils.h>
#include <UObject/ConstructorHelpers.h>

#include "UEProject/Spawner/UEProjectSpawnPoint.h"


AUEProjectGameMode::AUEProjectGameMode()
{
	// Set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(
		TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AUEProjectGameMode::BeginPlay()
{
	Super::BeginPlay();

	SpawnCharacters();
}

void AUEProjectGameMode::SpawnCharacters()
{
	// Iterate through each spawn point and spawn the character
	for (AUEProjectSpawnPoint* const SpawnPoint : FindActiveSpawnPoints())
	{
		if (SpawnPoint)
		{
			SpawnPoint->SpawnCharacter();
		}
	}
}

TArray<AUEProjectSpawnPoint*> AUEProjectGameMode::FindActiveSpawnPoints() const
{
	TArray<AUEProjectSpawnPoint*> ActiveSpawnPoints;

	// Iterate through all Spawn Point instances in the world
	for (TActorIterator<AUEProjectSpawnPoint> It(GetWorld()); It; ++It)
	{
		if (AUEProjectSpawnPoint* const SpawnPoint = *It)
		{
			ActiveSpawnPoints.Add(SpawnPoint);
		}
	}

	return ActiveSpawnPoints;
}
