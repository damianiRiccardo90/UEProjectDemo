#include "UEProjectNPCCharacter.h"

#include <Engine/StaticMesh.h>

#include "UEProject/AI/Controller/UEProjectAIController.h"


AUEProjectNPCCharacter::AUEProjectNPCCharacter()
	: AutoPossessAI(EAutoPossessAI::PlacedInWorldOrSpawned)
	, AIControllerClass(AUEProjectAIController::StaticClass())
#if WITH_EDITORONLY_DATA
	: PreviewPose(nullptr)
#endif // WITH_EDITORONLY_DATA
{}

#if WITH_EDITOR
UStaticMesh* AUEProjectNPCCharacter::GetPreviewPose() const
{
	return PreviewPose;
}
#endif // WITH_EDITOR
