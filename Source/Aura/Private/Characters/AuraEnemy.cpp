// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/AuraEnemy.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Aura/Aura.h"

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

void AAuraEnemy::HighlightActor()
{
	USkeletalMeshComponent* CharacterMesh = GetMesh();

	CharacterMesh->SetRenderCustomDepth(true);
	CharacterMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_HIGHLIGHT_VALUE);

	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_HIGHLIGHT_VALUE);
}

void AAuraEnemy::UnHighlightActor()
{
	USkeletalMeshComponent* CharacterMesh = GetMesh();
	
	CharacterMesh->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}
