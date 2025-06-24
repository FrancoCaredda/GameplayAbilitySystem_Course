// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;

// The player state is needed when there is the requirement of keeping all the abilities, cooldowns, stats of the controlled character
// even if it's got destroyed. The player state is associated with the controlled character via the GameMode
UCLASS()
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	AAuraPlayerState();
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	FORCEINLINE UAttributeSet* GetAttributeSet() const
	{
		return AttributeSet;
	}
protected:
	// The central object of GAS. Maintains ability granting, their activation/deactivation, and attributes.
	// Every class that has this component should implement the IAbilitySystemInterface
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	// An AttributeSet is a collection of attributes that define the character's stats
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
};
