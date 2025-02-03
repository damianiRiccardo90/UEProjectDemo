#pragma once

#include <CoreMinimal.h>

#include "UEProject/Characters/UEProjectBaseCharacter.h"

#include "UEProjectPlayerCharacter.generated.h"


class UGameplayCameraComponent;

UCLASS()
class AUEProjectPlayerCharacter : public AUEProjectBaseCharacter
{
	GENERATED_BODY()

public:

	AUEProjectPlayerCharacter(
		const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UGameplayCameraComponent* GetGameplayCameraComponent() const;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Character")
	UGameplayCameraComponent* GameplayCameraComponent;
};
