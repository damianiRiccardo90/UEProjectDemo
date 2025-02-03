#pragma once

#include <CoreMinimal.h>
#include <GameFramework/CharacterMovementComponent.h>

#include "UEProjectCharacterMovementComponent.generated.h"


UCLASS()
class UUEProjectCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:

	UUEProjectCharacterMovementComponent(
		const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:

	/** Default walking speed. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement")
    float DefaultWalkSpeed;
};
