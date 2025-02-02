#pragma once

#include <CoreMinimal.h>

#include "UEProject/Characters/UEProjectBaseCharacter.h"

#include "UEProjectPlayerCharacter.generated.h"


class UGameplayCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

UCLASS()
class AUEProjectPlayerCharacter : public AUEProjectBaseCharacter
{
	GENERATED_BODY()

public:

	AUEProjectPlayerCharacter(
		const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UGameplayCameraComponent* GetGameplayCamera() const;

protected:

	//////////////////////////////////////////////////////////////////////////
    // APawn overrides

	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// APawn overrides
	//////////////////////////////////////////////////////////////////////////

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Character")
	UGameplayCameraComponent* GameplayCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Character")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Character")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Character")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Character")
	UInputAction* LookAction;
};
