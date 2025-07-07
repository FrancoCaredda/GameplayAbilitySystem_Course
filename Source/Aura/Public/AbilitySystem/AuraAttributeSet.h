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

USTRUCT(BlueprintType)
struct FPostGameplayEffectProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAbilitySystemComponent* SourceAbilitySystemComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* SourceAvatarActor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AController* SourceController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ACharacter* SourceCharacter = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAbilitySystemComponent* TargetAbilitySystemComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* TargetAvatarActor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AController* TargetController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ACharacter* TargetCharacter = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayEffectContextHandle ContextHandle;
};

UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UAuraAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	
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
	// An attribute contains two floats: BaseValue and CurrentValue.
	// The BaseValue isn't the maximum value of this attribute.
	// The BaseValue is the permanent value of the attribute and serves for permanent changes, such as: receiving damage, healing, etc.
	// The CurrentValue is the temporary value of the attribute and serves for temporary changes, such as: buffs and debuffs.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes")
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Vital Attributes")
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Vital Attributes")
	FGameplayAttributeData Mana;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Vital Attributes")
	FGameplayAttributeData MaxMana;
private:
	FPostGameplayEffectProperties GetPostGameplayEffectProperties(const struct FGameplayEffectModCallbackData& Data) const;
};

