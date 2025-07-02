// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/AuraHUD.h"

#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"

UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WidgetControllerParams)
{
	if (!OverlayWidgetController)
	{
		OverlayWidgetController =
			NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WidgetControllerParams);
	}
	
	return OverlayWidgetController;
}

void AAuraHUD::InitOverlay(APlayerController* PlayerController, APlayerState* PlayerState,
	UAbilitySystemComponent* AbilitySystemComponent, UAttributeSet* AttributeSet)
{
	checkf(OverlayWidgetClass,
		TEXT("Overlay Widget Class uninitialized, please fill out AuraHUD"));
	checkf(OverlayWidgetControllerClass,
		TEXT("Overlay Widget Controller Class uninitialized, please fill out AuraHUD"));

	OverlayWidget = CreateWidget<UAuraUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget->AddToViewport();

	GetOverlayWidgetController(FWidgetControllerParams{PlayerController, PlayerState,
		AbilitySystemComponent, AttributeSet});

	OverlayWidget->SetWidgetController(OverlayWidgetController);
	OverlayWidgetController->BroadcastInitialValues();
	OverlayWidgetController->BindCallbacksToDependencies();
}

void AAuraHUD::BeginPlay()
{
	Super::BeginPlay();
}
