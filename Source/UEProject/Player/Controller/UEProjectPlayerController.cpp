#include "UEProjectPlayerController.h"

#include <AbilitySystemComponent.h>
#include <AbilitySystemGlobals.h>
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>
#include <GameFramework/Character.h>
#include <InputActionValue.h>
#include "Kismet/GameplayStatics.h"

#include "UEProject/GAS/UEProjectGameplayTagsLibrary.h"


AUEProjectPlayerController::AUEProjectPlayerController(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , DefaultMappingContext(nullptr)
    , JumpAction(nullptr)
    , MoveAction(nullptr)
    , LookAction(nullptr)
    , LightAttackAction(nullptr)
    , HeavyAttackAction(nullptr)
{}

void AUEProjectPlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    // Set this Pawn as the view target, ensuring we use its camera setup
    SetViewTarget(InPawn);

    // Setup input mapping context
    if (UEnhancedInputLocalPlayerSubsystem* const Subsystem = 
            ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        Subsystem->AddMappingContext(DefaultMappingContext, 0);
    }
}

void AUEProjectPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    UEnhancedInputComponent* const EnhancedInputComponent = 
        Cast<UEnhancedInputComponent>(InputComponent);
    if (!EnhancedInputComponent) return;

    // Jump
    EnhancedInputComponent->BindAction(
        JumpAction, ETriggerEvent::Started, this, &ThisClass::Jump);
    EnhancedInputComponent->BindAction(
        JumpAction, ETriggerEvent::Completed, this, &ThisClass::StopJumping);

    // Move
    EnhancedInputComponent->BindAction(
        MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);

    // Look
    EnhancedInputComponent->BindAction(
        LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);

    // Attack
    EnhancedInputComponent->BindAction(
        LightAttackAction, ETriggerEvent::Started, this, &ThisClass::LightAttack);
    EnhancedInputComponent->BindAction(
        HeavyAttackAction, ETriggerEvent::Started, this, &ThisClass::HeavyAttack);
}

void AUEProjectPlayerController::Jump(const FInputActionValue& Value)
{
    if (ACharacter* const CurrentCharacter = GetCharacter())
    {
        CurrentCharacter->Jump();
    }
}

void AUEProjectPlayerController::StopJumping(const FInputActionValue& Value)
{
    if (ACharacter* const CurrentCharacter = GetCharacter())
    {
        CurrentCharacter->StopJumping();
    }
}

void AUEProjectPlayerController::Move(const FInputActionValue& Value)
{
    ACharacter* const CurrentCharacter = GetCharacter();
    if (!CurrentCharacter) return;

    const APlayerCameraManager* const CameraManager =
        UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
    if (!CameraManager) return;

    const FRotator CameraRotation = CameraManager->GetCameraRotation();
    const FRotator YawRotation(0.f, CameraRotation.Yaw, 0.f);

    // Add movement vector debug
    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

    const FVector2D MovementVector = Value.Get<FVector2D>();

    CurrentCharacter->AddMovementInput(ForwardDirection, MovementVector.Y);
    CurrentCharacter->AddMovementInput(RightDirection, MovementVector.X);
}

void AUEProjectPlayerController::Look(const FInputActionValue& Value)
{
    const FVector2D LookAxisVector = Value.Get<FVector2D>();

    // Add yaw and pitch input
    AddYawInput(LookAxisVector.X);
    AddPitchInput(LookAxisVector.Y);
}

void AUEProjectPlayerController::LightAttack(const FInputActionValue& Value)
{
    APawn* const CurrentPawn = GetPawn();
    if (!CurrentPawn) return;

    if (UAbilitySystemComponent* const ASC = 
            UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(CurrentPawn))
    {
        FGameplayEventData EventData;
        EventData.EventTag = TAG_Ability_LightAttack;
        EventData.Instigator = CurrentPawn;
        EventData.Target = CurrentPawn;

        ASC->HandleGameplayEvent(TAG_Ability_LightAttack, &EventData);
    }
    /*
    FGameplayEventData EventData;
    EventData.EventTag = TAG_Ability_LightAttack;
    EventData.Instigator = CurrentPawn;
    EventData.Target = CurrentPawn;

    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
        CurrentPawn, TAG_Ability_LightAttack, EventData);
    */
}

void AUEProjectPlayerController::HeavyAttack(const FInputActionValue& Value)
{
    APawn* const CurrentPawn = GetPawn();
    if (!CurrentPawn) return;

    if (UAbilitySystemComponent* const ASC = 
            UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(CurrentPawn))
    {
        FGameplayEventData EventData;
        EventData.EventTag = TAG_Ability_HeavyAttack;
        EventData.Instigator = CurrentPawn;
        EventData.Target = CurrentPawn;

        ASC->HandleGameplayEvent(TAG_Ability_HeavyAttack, &EventData);
    }
    /*
    FGameplayEventData EventData;
    EventData.EventTag = TAG_Ability_HeavyAttack;
    EventData.Instigator = CurrentPawn;
    EventData.Target = CurrentPawn;

    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
        CurrentPawn, TAG_Ability_HeavyAttack, EventData);
    */
}
