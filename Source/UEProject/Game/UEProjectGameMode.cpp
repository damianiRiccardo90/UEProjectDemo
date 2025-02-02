#include "UEProjectGameMode.h"

#include <EngineUtils.h>
#include <UObject/ConstructorHelpers.h>

#include "UEProject/Spawner/UEProjectSpawnPoint.h"


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
