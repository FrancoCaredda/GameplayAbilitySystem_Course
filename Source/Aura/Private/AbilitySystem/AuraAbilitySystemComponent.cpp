// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemComponent.h"

void UAuraAbilitySystemComponent::InitComponent()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this,
		&UAuraAbilitySystemComponent::GameplayEffectApplied);
}

void UAuraAbilitySystemComponent::GameplayEffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
                                                        const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
	FGameplayTagContainer GameplayTags;
	GameplayEffectSpec.GetAllAssetTags(GameplayTags);

	OnGameplayTagRetrieved.Broadcast(GameplayTags);
}
