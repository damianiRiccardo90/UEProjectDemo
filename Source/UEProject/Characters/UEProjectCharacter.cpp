// Copyright Epic Games, Inc. All Rights Reserved.

#include "UEProjectCharacter.h"

#include <AbilitySystemComponent.h>
#include <Camera/CameraComponent.h>
#include <Components/CapsuleComponent.h>
#include <Engine/LocalPlayer.h>
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <GameFramework/Controller.h>
#include <GameFramework/SpringArmComponent.h>
#include <InputActionValue.h>

#include "UEProject/GAS/Attributes/UEProjectAttributeSet_CharacterBase.h"


DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AUEProjectCharacter::AUEProjectCharacter()
	: GameplayCamera(nullptr)
	, AbilitySystem(nullptr)
	, DefaultMappingContext(nullptr)
	, JumpAction(nullptr)
	, MoveAction(nullptr)
	, LookAction(nullptr)
	, BaseAttributeSet(nullptr)
#if WITH_EDITORONLY_DATA
	, PreviewPose(nullptr)
#endif
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	GameplayCamera = CreateDefaultSubobject<UGameplayCameraComponent>(TEXT("GameplayCamera"));
	GameplayCamera->SetupAttachment(RootComponent);

	AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));

	// Create the attribute set and make sure it's "owned" by the ASC, so it can replicate properly.
    BaseAttributeSet = CreateDefaultSubobject<UUEProjectAttributeSet_CharacterBase>(TEXT("BaseAttributeSet"));

    // Register the attribute set with the ASC
    if (AbilitySystem && BaseAttributeSet)
    {
        AbilitySystem->AddAttributeSetSubobject(BaseAttributeSet);
    }

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

UGameplayCameraComponent* AUEProjectCharacter::GetGameplayCamera() const
{
	return GameplayCamera;
}

UAbilitySystemComponent* AUEProjectCharacter::GetAbilitySystem() const
{
	return AbilitySystem;
}

UUEProjectAttributeSet_CharacterBase* AUEProjectCharacter::GetBaseAttributeSet() const
{
	return BaseAttributeSet;
}

#if WITH_EDITORONLY_DATA
UStaticMesh* AUEProjectCharacter::GetPreviewPose() const
{
	return PreviewPose;
}
#endif

void AUEProjectCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* const PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* const Subsystem = 
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AUEProjectCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* const EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AUEProjectCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AUEProjectCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error,
			   TEXT("'%s' Failed to find an Enhanced Input component! ")
			   TEXT("This template is built to use the Enhanced Input system. ")
			   TEXT("If you intend to use the legacy system, then you will need to update this C++ file."),
			   *GetNameSafe(this));
	}
}

void AUEProjectCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (!Controller)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AUEProjectCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (!Controller)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}
