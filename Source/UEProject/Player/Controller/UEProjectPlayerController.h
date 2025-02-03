#pragma once

#include <CoreMinimal.h>
#include <GameFramework/PlayerController.h>

#include "UEProjectPlayerController.generated.h"


class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

UCLASS()
class AUEProjectPlayerController : public APlayerController
{
    GENERATED_BODY()

public:

    AUEProjectPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:

    //////////////////////////////////////////////////////////////////////////
    // APlayerController overrides

    virtual void OnPossess(APawn* InPawn) override;
    virtual void SetupInputComponent() override;

    // APlayerController overrides
    //////////////////////////////////////////////////////////////////////////

    void Jump(const FInputActionValue& Value);
    void StopJumping(const FInputActionValue& Value);
    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Controller")
    UInputMappingContext* DefaultMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Controller")
    UInputAction* JumpAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Controller")
    UInputAction* MoveAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Controller")
    UInputAction* LookAction;
};
