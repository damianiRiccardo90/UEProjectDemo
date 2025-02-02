#pragma once

#include <CoreMinimal.h>

#include "UEProject/Characters/UEProjectBaseCharacter.h"

#include "UEProjectNPCCharacter.generated.h"


class UStaticMesh;

UCLASS()
class AUEProjectNPCCharacter : public AUEProjectBaseCharacter
{
	GENERATED_BODY()

public:

	AUEProjectNPCCharacter();

#if WITH_EDITOR
    UStaticMesh* GetPreviewPose() const;
#endif // WITH_EDITOR

protected:

#if WITH_EDITORONLY_DATA
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPC Character")
    UStaticMesh* PreviewPose;
#endif // WITH_EDITORONLY_DATA
};
