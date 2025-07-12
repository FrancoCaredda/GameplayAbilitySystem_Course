// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/MMC/MMCMaxHealth.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interface/CombatInterface.h"

UMMCMaxHealth::UMMCMaxHealth()
{
	VigorDefinition.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();
	VigorDefinition.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	// Whether the attribute should be captured when the GameplayEffectSpec is created or when it's applied.
	// True means at the creation time.
	VigorDefinition.bSnapshot = false;

	RelevantAttributesToCapture.Add(VigorDefinition);
}

float UMMCMaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParams;
	EvaluationParams.SourceTags = SourceTags;
	EvaluationParams.TargetTags = TargetTags;

	float Magnitude = 0;
	GetCapturedAttributeMagnitude(VigorDefinition, Spec, EvaluationParams, Magnitude);

	Magnitude = FMath::Max(Magnitude, 0.0f);

	const ICombatInterface* CombatInterface =
		Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());

	if (CombatInterface)
	{
		const int32 PlayerLevel = CombatInterface->GetPlayerLevel();
	
		return 80.0f + 2.5f * Magnitude + 10.0f * PlayerLevel;
	}

	return 80.0f + 2.5f * Magnitude + 10.0f;
}
