// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/AuraEnemy.h"

#include "Aura/Aura.h"

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
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
