#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Character.h>

#include "UEProject/GAS/Attributes/UEProjectAttributeSet_CharacterBase.h"

#include "UEProjectBaseCharacter.generated.h"


class UAbilitySystemComponent;
class UUEProjectAttributeSet_CharacterBase;
class UUEProjectCharacterMovementComponent;
class UUEProjectGameplayAbilitySet;

UCLASS()
class AUEProjectBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	AUEProjectBaseCharacter(const FObjectInitializer& ObjectInitializer);

    //////////////////////////////////////////////////////////////////////////
	// ACharacter overrides

	virtual void BeginPlay() override;

	// ACharacter overrides
    //////////////////////////////////////////////////////////////////////////

	UAbilitySystemComponent* GetAbilitySystem() const;
    UUEProjectAttributeSet_CharacterBase* GetBaseAttributeSet() const;
    UUEProjectCharacterMovementComponent* GetCustomMovementComponent() const;

protected:

	UFUNCTION()
	void OnGameplayEffectApplied(UAbilitySystemComponent* Source, const FGameplayEffectSpec& Spec, 
		FActiveGameplayEffectHandle Handle);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base Character")
	UAbilitySystemComponent* AbilitySystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base Character")
    UUEProjectAttributeSet_CharacterBase* BaseAttributeSet;

	/** Reference to an ability set data asset holding the granted abilities by default. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base Character")
    UUEProjectGameplayAbilitySet* DefaultAbilitySet;
};
