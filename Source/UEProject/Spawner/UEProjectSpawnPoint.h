#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Actor.h>

#include "UEProject/AI/Controller/UEProjectAIController.h"
#include "UEProject/Characters/UEProjectCharacter.h"

#include "UEProjectSpawnPoint.generated.h"


class UArrowComponent;
class UBillboardComponent;

/**
 * A simple actor that acts as a spawn point for characters in the game world.
 */
UCLASS(Blueprintable, Category = "Spawner", meta = (DisplayName = "Spawn Point"))
class AUEProjectSpawnPoint : public AActor
{
	GENERATED_BODY()

public:

	AUEProjectSpawnPoint();

	// Spawns the character by using the arrow component rotation to set the character initial facing
	void SpawnCharacter();

protected:

#if WITH_EDITOR
	//////////////////////////////////////////////////////////////////////////
    // UObject overrides

    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	// UObject overrides
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
    // AActor overrides

	virtual void PostEditMove(bool bFinished) override;

	// AActor overrides
	//////////////////////////////////////////////////////////////////////////

	// Update the preview mesh based on the set CharacterClass
	void UpdatePreviewMesh();

    // Check for collisions and update the material
    void UpdateMaterialBasedOnCollision();
#endif // WITH_EDITOR

	/** The arrow component to indicate the spawn direction. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawn")
	UArrowComponent* ArrowComponent;

	/** The class of the character to spawn. */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Spawn")
	TSubclassOf<AUEProjectCharacter> CharacterClass;

	/** The class of the AI controller that possesses the character. */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Spawn")
	TSubclassOf<AUEProjectAIController> AIControllerClass;

	/** Whether this spawn point is enabled. */
    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Spawn", 
			  meta = (DisplayName = "Is Enabled"))
    bool bIsEnabled = true;

#if WITH_EDITORONLY_DATA
	UPROPERTY()
	UBillboardComponent* IconBillboard;

    UPROPERTY()
    UStaticMeshComponent* PreviewMeshComponent;

	/** Material applied to the preview mesh, used when spawn point is correctly placed. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn")
	UMaterialInterface* CorrectPlacementMaterial;

	/** Material applied to the preview mesh, used when spawn point is colliding with something. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn")
	UMaterialInterface* WrongPlacementMaterial;
#endif // WITH_EDITORONLY_DATA
};
