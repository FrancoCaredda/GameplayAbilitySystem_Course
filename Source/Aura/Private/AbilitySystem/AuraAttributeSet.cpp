// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	InitHealth(50.0f);
	InitMaxHealth(100.0f);
	InitMana(25.0f);
	InitMaxMana(50.0f);
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
}

// TODO: Debug on multiple gameplay effects
void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	else if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxMana());
	}
}

void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FPostGameplayEffectProperties EffectProperties = GetPostGameplayEffectProperties(Data);
}

void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana);
}

FPostGameplayEffectProperties UAuraAttributeSet::GetPostGameplayEffectProperties(
	const FGameplayEffectModCallbackData& Data) const
{
	FPostGameplayEffectProperties Result{};

	const FGameplayEffectSpec& GameplayEffectSpec = Data.EffectSpec;

	Result.ContextHandle = GameplayEffectSpec.GetContext();
	Result.SourceAbilitySystemComponent =
		Result.ContextHandle.GetInstigatorAbilitySystemComponent();

	if (Result.SourceAbilitySystemComponent)
	{
		Result.SourceAvatarActor = Result.SourceAbilitySystemComponent->GetAvatarActor();
		Result.SourceCharacter   = Cast<ACharacter>(Result.SourceAvatarActor);
		Result.SourceController  = Result.SourceAbilitySystemComponent->
										 AbilityActorInfo->PlayerController.Get();
		
		if (Result.SourceCharacter && !Result.SourceController)
		{
			Result.SourceController = Result.SourceCharacter->GetController();
		}
	}
	
	Result.TargetAbilitySystemComponent = &Data.Target;

	if (Result.TargetAbilitySystemComponent)
	{
		Result.TargetAvatarActor = Result.TargetAbilitySystemComponent->GetAvatarActor();
		Result.TargetCharacter   = Cast<ACharacter>(Result.TargetAvatarActor);
		Result.TargetController  = Result.TargetAbilitySystemComponent->
										 AbilityActorInfo->PlayerController.Get();
		
		if (Result.TargetCharacter && !Result.TargetController)
		{
			Result.TargetController = Result.TargetCharacter->GetController();
		}
	}
	
	return Result;
}
