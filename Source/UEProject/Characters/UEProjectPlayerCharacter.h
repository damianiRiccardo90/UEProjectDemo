#pragma once

#include <CoreMinimal.h>
#include <GenericTeamAgentInterface.h>

#include "UEProject/Characters/UEProjectBaseCharacter.h"

#include "UEProjectPlayerCharacter.generated.h"


class UAIPerceptionStimuliSourceComponent;
class UGameplayCameraComponent;

UCLASS()
class AUEProjectPlayerCharacter : 
	public AUEProjectBaseCharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:

	AUEProjectPlayerCharacter(
		const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//////////////////////////////////////////////////////////////////////////
	// IGenericTeamAgentInterface overrides

	virtual FGenericTeamId GetGenericTeamId() const override;

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	// IGenericTeamAgentInterface overrides
	//////////////////////////////////////////////////////////////////////////

	UGameplayCameraComponent* GetGameplayCameraComponent() const;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Character")
	UGameplayCameraComponent* GameplayCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Character")
    UAIPerceptionStimuliSourceComponent* PerceptionStimuliSource;
};
