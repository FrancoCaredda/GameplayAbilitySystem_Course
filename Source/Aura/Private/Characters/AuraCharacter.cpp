// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	SpringArmComponent->bAutoActivate = false;
	SpringArmComponent->bInheritPitch = false;
	SpringArmComponent->bInheritRoll = false;
	SpringArmComponent->bInheritYaw = false;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->AttachToComponent(SpringArmComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CameraComponent->bUsePawnControlRotation = false;

	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	CharacterMovementComponent->bOrientRotationToMovement = true;
	CharacterMovementComponent->RotationRate = FRotator(0.0f, 400.0f, 0.0f);
	CharacterMovementComponent->bConstrainToPlane = true;
	CharacterMovementComponent->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

// This function is called when the character is possessed on the server
// Here we set up the AbilitySystemComponent for the server
void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();
	InitHUD();
}

// This function is called when the player state associated with this character has been replicated to the client
// Here we set up the AbilitySystemComponent for the client
void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitAbilityActorInfo();
	InitHUD();
}

void AAuraCharacter::InitAbilityActorInfo()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet = AuraPlayerState->GetAttributeSet();

	// This function provides the information about the owner of the component to the AbilitySystemComponent
	// There are two types of owners: a physical one and an avatar.
	// The physical owner is the actor that contains the component (in our case the PlayerState)
	// The avatar is the actor that will operate with this component (in out case the Character)
	// However there are cases when both of these pointers point to the same object (for ex. AI controlled characters)
	AbilitySystemComponent->InitAbilityActorInfo(AuraPlayerState, this);
}

void AAuraCharacter::InitHUD() const
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);

	// We have to check if this is valid, because controllers of the other characters won't be available to us
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		// HUD is only valid for the local player, not server
		if (AAuraHUD* HUD = Cast<AAuraHUD>(PlayerController->GetHUD()))
		{
			HUD->InitOverlay(PlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
}
