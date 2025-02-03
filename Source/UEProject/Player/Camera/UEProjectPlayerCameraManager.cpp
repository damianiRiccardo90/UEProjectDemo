#include "UEProjectPlayerCameraManager.h"

#include <Camera/CameraComponent.h>
#include <GameFramework/GameplayCameraComponent.h>
#include <GameFramework/SpringArmComponent.h>

#include "UEProject/Characters/UEProjectPlayerCharacter.h"


AUEProjectPlayerCameraManager::AUEProjectPlayerCameraManager(
    const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{}

void AUEProjectPlayerCameraManager::InitializeFor(APlayerController* PC)
{
    // Skip parent InitializeFor to prevent default camera setup
    PCOwner = PC;

    // Now set our motherfucking gameplay camera on the character through the set view target loop
    if (APawn* const Pawn = PCOwner->GetPawn())
    {
        SetViewTarget(Pawn);
    }
}

/*
void AUEProjectPlayerCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
    // If we have no view target, fall back to parent implementation
    if (!OutVT.Target)
    {
        Super::UpdateViewTarget(OutVT, DeltaTime);
        return;
    }

    // Try to get our player character
    AUEProjectPlayerCharacter* const PlayerCharacter = Cast<AUEProjectPlayerCharacter>(OutVT.Target);
    if (!PlayerCharacter)
    {
        Super::UpdateViewTarget(OutVT, DeltaTime);
        return;
    }

    // Get camera component's transform and properties
    if (UGameplayCameraComponent* const GameplayCamera = 
            PlayerCharacter->GetGameplayCameraComponent())
    {
        OutVT.POV.Location = GameplayCamera->GetComponentLocation();
        OutVT.POV.Rotation = GameplayCamera->GetComponentRotation();
        OutVT.POV.FOV = GameplayCamera->FieldOfView;
    }
}
*/
