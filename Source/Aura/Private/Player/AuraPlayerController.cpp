// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"

AAuraPlayerController::AAuraPlayerController()
{
	// A change of the instance on the server will
	// be replicated (sent) to all clients 
	bReplicates = true;
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(AuraInputContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	check(Subsystem);
	Subsystem->AddMappingContext(AuraInputContext, 0);

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	// This structure permits us to override the mouse cursor's behaviour
	FInputModeGameAndUI GameAndUI;
	GameAndUI.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	GameAndUI.SetHideCursorDuringCapture(false);
	SetInputMode(GameAndUI);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent =
		CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this,
		&AAuraPlayerController::OnMoveActionTriggered);
}

void AAuraPlayerController::OnMoveActionTriggered(const FInputActionValue& InputActionValue)
{
	const FVector2D Direction = InputActionValue.Get<FVector2D>();
	const FRotator ControllerRotation = FRotator(0.0f, GetControlRotation().Yaw, 0.0);

	const FRotationMatrix RotationMatrix(ControllerRotation);
	
	const FVector ForwardVector = RotationMatrix.GetUnitAxis(EAxis::X);
	const FVector RightVector = RotationMatrix.GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardVector, Direction.Y);
		ControlledPawn->AddMovementInput(RightVector, Direction.X);
	}
}
