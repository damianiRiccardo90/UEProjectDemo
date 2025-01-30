#include "UEProjectSpawnPoint.h"

#include <Components/ArrowComponent.h>
#include <Components/BillboardComponent.h>
#include <Engine/OverlapResult.h>
#include <Engine/Texture2D.h>


AUEProjectSpawnPoint::AUEProjectSpawnPoint() :
#if WITH_EDITORONLY_DATA
    IconBillboard(nullptr),
    PreviewMeshComponent(nullptr),
    CorrectPlacementMaterial(nullptr),
    WrongPlacementMaterial(nullptr),
#endif
    ArrowComponent(nullptr),
	CharacterClass(nullptr),
    bIsEnabled(true)
{
	// Enable ticking if needed
	PrimaryActorTick.bCanEverTick = false;

#if !WITH_EDITOR
    // If we're NOT in the editor, use a plain SceneComponent as root
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
#else
    // If we're in the editor, use a BillboardComponent as root to show an icon
    IconBillboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("SpawnPointIcon"));
    RootComponent = IconBillboard;

    static ConstructorHelpers::FObjectFinder<UTexture2D> IconFinder(
        TEXT("/Engine/EditorResources/S_PlaceActor.S_PlaceActor")
    );
    if (IconFinder.Succeeded())
    {
        IconBillboard->SetSprite(IconFinder.Object);
    }

    // Create and configure the preview mesh component
    PreviewMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PreviewMeshComponent"));
    PreviewMeshComponent->SetupAttachment(RootComponent);
    PreviewMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    PreviewMeshComponent->SetVisibility(false); // Initially hidden
#endif

    // Create and configure an arrow component to indicate the spawn direction
    ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
    ArrowComponent->SetupAttachment(RootComponent);
    ArrowComponent->SetArrowColor(FColor::Green);
    ArrowComponent->SetArrowLength(100.0f);
    ArrowComponent->SetHiddenInGame(false); // Ensure the arrow is visible only in the editor
}

void AUEProjectSpawnPoint::SpawnCharacter()
{
    if (!bIsEnabled) return;

    // Get the spawn transform from the arrow component
    const FVector SpawnLocation = GetActorLocation();
    const FRotator SpawnRotation = ArrowComponent->GetComponentRotation();

    // Define spawn parameters
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.Instigator = nullptr;
    SpawnParams.SpawnCollisionHandlingOverride = 
        ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

    // Spawn the character
    GetWorld()->SpawnActor<ACharacter>(CharacterClass, SpawnLocation, 
                                       SpawnRotation, SpawnParams);
}

#if WITH_EDITORONLY_DATA
void AUEProjectSpawnPoint::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    if (PropertyChangedEvent.Property && 
        PropertyChangedEvent.Property->GetFName() == 
            GET_MEMBER_NAME_CHECKED(AUEProjectSpawnPoint, CharacterClass))
    {
        UpdatePreviewMesh();
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
    if (CharacterClass)
    {
        if (const AUEProjectCharacter* const DefaultCharacter =
            CharacterClass->GetDefaultObject<AUEProjectCharacter>())
        {
            if (UStaticMesh* const CharacterPreviewMesh = DefaultCharacter->GetPreviewStaticMesh())
            {
                if (PreviewMeshComponent)
                {
                    PreviewMeshComponent->SetStaticMesh(CharacterPreviewMesh);
                    PreviewMeshComponent->SetVisibility(true);
                }
            }
        }
    }
    else
    {
        // Hide the preview mesh if CharacterClass is null
        PreviewMeshComponent->SetVisibility(false);
    }
}

void AUEProjectSpawnPoint::UpdateMaterialBasedOnCollision()
{
    if (!PreviewMeshComponent || !CorrectPlacementMaterial || !WrongPlacementMaterial)
    {
        return;
    }

    // Check if the preview mesh is colliding with anything
    TArray<FOverlapResult> Overlaps;
    FCollisionQueryParams CollisionParams;
    CollisionParams.bTraceComplex = true;
    CollisionParams.AddIgnoredActor(this);

    const bool bIsColliding = GetWorld()->OverlapMultiByChannel(
        Overlaps,
        PreviewMeshComponent->GetComponentLocation(),
        PreviewMeshComponent->GetComponentQuat(),
        ECC_WorldStatic,
        FCollisionShape::MakeBox(PreviewMeshComponent->Bounds.BoxExtent),
        CollisionParams
    );

    // Apply the appropriate material based on collision
    if (bIsColliding)
    {
        PreviewMeshComponent->SetMaterial(0, WrongPlacementMaterial);
    }
    else
    {
        PreviewMeshComponent->SetMaterial(0, CorrectPlacementMaterial);
    }
}
#endif
