#include "UEProjectSpawnPoint.h"

#include <Components/ArrowComponent.h>
#include <Components/BillboardComponent.h>
#include <Components/CapsuleComponent.h>
#include <Engine/OverlapResult.h>
#include <Engine/Texture2D.h>


AUEProjectSpawnPoint::AUEProjectSpawnPoint() :
    ArrowComponent(nullptr),
	CharacterClass(nullptr),
    bIsEnabled(true),
#if WITH_EDITORONLY_DATA
    IconBillboard(nullptr),
    PreviewMeshComponent(nullptr),
    CorrectPlacementMaterial(nullptr),
    WrongPlacementMaterial(nullptr)
#endif
{
    // Allow ticking if needed (off by default)
    PrimaryActorTick.bCanEverTick = false;

    // Create and configure an arrow component to indicate the spawn transform
    ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
    RootComponent = ArrowComponent;
    ArrowComponent->SetArrowColor(FColor::Green);
    ArrowComponent->SetArrowLength(100.f);
    // Hide the arrow at runtime, but keep it visible in the editor
    ArrowComponent->SetHiddenInGame(true, true);

#if WITH_EDITOR
    // Create the Billboard icon and attach it to the arrow
    IconBillboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("SpawnPointIcon"));
    IconBillboard->SetupAttachment(ArrowComponent);

    static ConstructorHelpers::FObjectFinder<UTexture2D> IconFinder(
        TEXT("/Engine/EditorResources/S_PlaceActor.S_PlaceActor")
    );
    if (IconFinder.Succeeded())
    {
        IconBillboard->SetSprite(IconFinder.Object);
    }

    // Create and configure the preview mesh component
    PreviewMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PreviewMeshComponent"));
    PreviewMeshComponent->SetupAttachment(ArrowComponent);
    PreviewMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    PreviewMeshComponent->SetVisibility(false); // Initially hidden
#endif
}

void AUEProjectSpawnPoint::SpawnCharacter()
{
    if (!bIsEnabled) return;

    // Get the spawn transform from the arrow component
    const FTransform SpawnTransform = ArrowComponent->GetComponentTransform();

    // Define spawn parameters
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.Instigator = nullptr;
    SpawnParams.SpawnCollisionHandlingOverride = 
        ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

    // Spawn the character
    GetWorld()->SpawnActor<ACharacter>(CharacterClass, SpawnTransform, SpawnParams);
}

#if WITH_EDITOR
void AUEProjectSpawnPoint::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    if (PropertyChangedEvent.Property && 
        PropertyChangedEvent.Property->GetFName() == 
            GET_MEMBER_NAME_CHECKED(AUEProjectSpawnPoint, CharacterClass))
    {
        UpdatePreviewMesh();

        // Update the material based on collision after altering the character class
        UpdateMaterialBasedOnCollision();
    }
}

void AUEProjectSpawnPoint::PostEditMove(bool bFinished)
{
    Super::PostEditMove(bFinished);

    // Update the material based on collision after moving the actor
    UpdateMaterialBasedOnCollision();
}

void AUEProjectSpawnPoint::UpdatePreviewMesh()
{
    if (!PreviewMeshComponent) return;

    if (CharacterClass)
    {
        if (const AUEProjectCharacter* const DefaultCharacter =
                CharacterClass->GetDefaultObject<AUEProjectCharacter>())
        {
            if (UStaticMesh* const CharacterPreviewMesh = DefaultCharacter->GetPreviewPose())
            {
                // Assign the mesh
                PreviewMeshComponent->SetStaticMesh(CharacterPreviewMesh);

                // Match the preview mesh's relative transform (to the direction arrow) 
                // to the character's skeletal mesh component (to the parent character position), 
                // so that the preview mesh is visually aligned with the arrow
                if (const USkeletalMeshComponent* const CharSkeletalMeshComponent = 
                        DefaultCharacter->GetMesh())
                {
                    PreviewMeshComponent->SetRelativeTransform(
                        CharSkeletalMeshComponent->GetRelativeTransform());
                }

                // Show the preview mesh, hide the icon
                PreviewMeshComponent->SetVisibility(true);
                if (IconBillboard)
                {
                    IconBillboard->SetVisibility(false);
                }
            }
        }
    }
    else
    {
        // Hide the preview mesh if CharacterClass is null
        PreviewMeshComponent->SetVisibility(false);
        // Show the icon again
        if (IconBillboard)
        {
            IconBillboard->SetVisibility(true);
        }
    }
}

void AUEProjectSpawnPoint::UpdateMaterialBasedOnCollision()
{
    if (!PreviewMeshComponent || !CorrectPlacementMaterial || 
        !WrongPlacementMaterial || !CharacterClass)
    {
        return;
    }

    // Get the capsule from the default character
    const UCapsuleComponent* const DefaultCapsule = 
        CharacterClass->GetDefaultObject<AUEProjectCharacter>()->GetCapsuleComponent();

    // Simple overlap test using character's capsule dimensions at the spawn point location
    TArray<FOverlapResult> Overlaps;
    const bool bIsColliding = GetWorld()->OverlapMultiByChannel(
        Overlaps,
        GetActorLocation(),  // Where the character will spawn
        GetActorRotation().Quaternion(),  // How it will be oriented
        ECC_WorldStatic,
        FCollisionShape::MakeCapsule(
            DefaultCapsule->GetScaledCapsuleRadius(),
            DefaultCapsule->GetScaledCapsuleHalfHeight()
        ),
        FCollisionQueryParams(NAME_None, false, this)
    );

    PreviewMeshComponent->SetMaterial(
        0, bIsColliding ? WrongPlacementMaterial : CorrectPlacementMaterial);
}
#endif
