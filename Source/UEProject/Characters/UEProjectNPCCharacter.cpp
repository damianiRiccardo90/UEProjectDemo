#include "UEProjectNPCCharacter.h"

#include <Components/SkeletalMeshComponent.h>
#include <Engine/StaticMesh.h>

#include "UEProject/AI/Controller/UEProjectAIController.h"


AUEProjectNPCCharacter::AUEProjectNPCCharacter(
	const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
#if WITH_EDITORONLY_DATA
	, PreviewPose(nullptr)
#endif // WITH_EDITORONLY_DATA
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AUEProjectAIController::StaticClass();

	BaseEyeHeight = 64.f;

	if (USkeletalMeshComponent* const SkeletalMeshComponent = GetMesh())
	{
		// Allows to be perceived by the sight sense
		SkeletalMeshComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	}
}

FGenericTeamId AUEProjectNPCCharacter::GetGenericTeamId() const
{
	return 1;
}

ETeamAttitude::Type AUEProjectNPCCharacter::GetTeamAttitudeTowards(
	const AActor& Other) const
{
	const IGenericTeamAgentInterface* const OtherTeamAgent = 
		Cast<IGenericTeamAgentInterface>(&Other);
	if (!OtherTeamAgent) return ETeamAttitude::Neutral;

	const uint8 OtherAgentTeamID = OtherTeamAgent->GetGenericTeamId().GetId();

	if (OtherAgentTeamID == 1) return ETeamAttitude::Friendly;
	if (OtherAgentTeamID == 0) return ETeamAttitude::Hostile;

	return ETeamAttitude::Neutral;
}

#if WITH_EDITOR
UStaticMesh* AUEProjectNPCCharacter::GetPreviewPose() const
{
	return PreviewPose;
}
#endif // WITH_EDITOR
