// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include <AbilitySystemComponent.h>
#include <CoreMinimal.h>
#include <GameFramework/Character.h>
#include <GameFramework/GameplayCameraComponent.h>
#include <Logging/LogMacros.h>

#include "UEProject/GAS/Attributes/UEProjectAttributeSet_CharacterBase.h"

#include "UEProjectCharacter.generated.h"


class UInputAction;
class UInputMappingContext;
class UUEProjectAttributeSet_CharacterBase;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AUEProjectCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	AUEProjectCharacter();

	UGameplayCameraComponent* GetGameplayCamera() const;
	UAbilitySystemComponent* GetAbilitySystem() const;
    UUEProjectAttributeSet_CharacterBase* GetBaseAttributeSet() const;
#if WITH_EDITORONLY_DATA
    UStaticMesh* GetPreviewStaticMesh() const;
#endif

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UGameplayCameraComponent* GameplayCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay")
	UAbilitySystemComponent* AbilitySystem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Attributes")
    UUEProjectAttributeSet_CharacterBase* BaseAttributeSet;

#if WITH_EDITORONLY_DATA
    UPROPERTY(EditDefaultsOnly, Category = "Preview")
    UStaticMesh* PreviewStaticMesh;
#endif
};
