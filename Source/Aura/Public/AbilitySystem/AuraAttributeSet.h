// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AuraAttributeSet.generated.h"

// To add an attribute follow these steps
// 1. Add a member variable of type FGameplayAttributeData and mark it as replicated
// 2. Register it for replication in GetLifetimeReplicatedProps
// 3. Notify the GAS via the RepNotify OnRep_<variable_name>

UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UAuraAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	ATTRIBUTE_ACCESSORS_BASIC(UAuraAttributeSet, Health)
	
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	ATTRIBUTE_ACCESSORS_BASIC(UAuraAttributeSet, MaxHealth)

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

	ATTRIBUTE_ACCESSORS_BASIC(UAuraAttributeSet, Mana)
	
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;

	ATTRIBUTE_ACCESSORS_BASIC(UAuraAttributeSet, MaxMana)
public:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes")
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Vital Attributes")
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Vital Attributes")
	FGameplayAttributeData Mana;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Vital Attributes")
	FGameplayAttributeData MaxMana;
};

