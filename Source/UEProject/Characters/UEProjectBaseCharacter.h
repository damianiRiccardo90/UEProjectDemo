#pragma once

#include <AbilitySystemInterface.h>
#include <CoreMinimal.h>
#include <GameplayTagAssetInterface.h>
#include <GameFramework/Character.h>

#include "UEProject/GAS/Attributes/UEProjectAttributeSet_CharacterBase.h"

#include "UEProjectBaseCharacter.generated.h"


class UAbilitySystemComponent;
class UUEProjectAttributeSet_CharacterBase;
class UUEProjectCharacterMovementComponent;
class UUEProjectGameplayAbilitySet;

UCLASS()
class AUEProjectBaseCharacter : 
	public ACharacter, public IAbilitySystemInterface, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:

	AUEProjectBaseCharacter(const FObjectInitializer& ObjectInitializer);

    //////////////////////////////////////////////////////////////////////////
	// ACharacter overrides

	virtual void BeginPlay() override;

	// ACharacter overrides
    //////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// IAbilitySystemInterface overrides

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// IAbilitySystemInterface overrides
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// IGameplayTagAssetInterface overrides

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

	UFUNCTION(BlueprintCallable, Category=GameplayTags)
	virtual bool HasMatchingGameplayTag(FGameplayTag TagToCheck) const override;

	UFUNCTION(BlueprintCallable, Category=GameplayTags)
	virtual bool HasAllMatchingGameplayTags(
		const FGameplayTagContainer& TagContainer) const override;

	UFUNCTION(BlueprintCallable, Category=GameplayTags)
	virtual bool HasAnyMatchingGameplayTags(
		const FGameplayTagContainer& TagContainer) const override;

	// IGameplayTagAssetInterface overrides
	//////////////////////////////////////////////////////////////////////////

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
