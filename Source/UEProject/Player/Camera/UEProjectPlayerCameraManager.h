#pragma once

#include <Camera/PlayerCameraManager.h>
#include <CoreMinimal.h>

#include "UEProjectPlayerCameraManager.generated.h"


UCLASS()
class AUEProjectPlayerCameraManager : public APlayerCameraManager
{
    GENERATED_BODY()

public:

    AUEProjectPlayerCameraManager(
        const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    virtual void InitializeFor(class APlayerController* PC) override;

protected:

    //////////////////////////////////////////////////////////////////////////
    // APlayerCameraManager overrides

    //virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;

    // APlayerCameraManager overrides
    //////////////////////////////////////////////////////////////////////////
};
