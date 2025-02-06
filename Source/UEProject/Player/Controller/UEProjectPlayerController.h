#pragma once

#include <CoreMinimal.h>
#include <GameFramework/PlayerController.h>
#include <GenericTeamAgentInterface.h>

#include "UEProjectPlayerController.generated.h"


class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

UCLASS()
class AUEProjectPlayerController : public APlayerController, public IGenericTeamAgentInterface
{
    GENERATED_BODY()

public:

    AUEProjectPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    //////////////////////////////////////////////////////////////////////////
    // IGenericTeamAgentInterface overrides

	virtual FGenericTeamId GetGenericTeamId() const override;

    virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

    // IGenericTeamAgentInterface overrides
    //////////////////////////////////////////////////////////////////////////

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
    void LightAttack(const FInputActionValue& Value);
    void HeavyAttack(const FInputActionValue& Value);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Controller")
    UInputMappingContext* DefaultMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Controller")
    UInputAction* JumpAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Controller")
    UInputAction* MoveAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Controller")
    UInputAction* LookAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Controller")
    UInputAction* LightAttackAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Controller")
    UInputAction* HeavyAttackAction;
};
