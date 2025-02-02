#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Character.h>

#include "UEProject/GAS/Attributes/UEProjectAttributeSet_CharacterBase.h"

#include "UEProjectBaseCharacter.generated.h"


class UAbilitySystemComponent;
class UUEProjectAttributeSet_CharacterBase;

UCLASS()
class AUEProjectBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	AUEProjectBaseCharacter();

	UAbilitySystemComponent* GetAbilitySystem() const;
    UUEProjectAttributeSet_CharacterBase* GetBaseAttributeSet() const;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base Character")
	UAbilitySystemComponent* AbilitySystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Base Character")
    UUEProjectAttributeSet_CharacterBase* BaseAttributeSet;
};
