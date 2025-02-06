#pragma once

#include <CoreMinimal.h>
#include <GenericTeamAgentInterface.h>

#include "UEProject/Characters/UEProjectBaseCharacter.h"

#include "UEProjectNPCCharacter.generated.h"


class UStaticMesh;

UCLASS()
class AUEProjectNPCCharacter : 
    public AUEProjectBaseCharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:

	AUEProjectNPCCharacter(
        const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    //////////////////////////////////////////////////////////////////////////
    // IGenericTeamAgentInterface overrides

    virtual FGenericTeamId GetGenericTeamId() const override;

    virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

    // IGenericTeamAgentInterface overrides
    //////////////////////////////////////////////////////////////////////////

#if WITH_EDITOR
    UStaticMesh* GetPreviewPose() const;
#endif // WITH_EDITOR

protected:

#if WITH_EDITORONLY_DATA
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPC Character")
    UStaticMesh* PreviewPose;
#endif // WITH_EDITORONLY_DATA
};
