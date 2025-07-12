// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/MMC/MMCMaxMana.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interface/CombatInterface.h"

UMMCMaxMana::UMMCMaxMana()
{
	IntelligenceCapture.AttributeToCapture = UAuraAttributeSet::GetIntelligenceAttribute();
	IntelligenceCapture.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntelligenceCapture.bSnapshot = false;

	RelevantAttributesToCapture.Add(IntelligenceCapture);
}

float UMMCMaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTagContainer = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTagContainer = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTagContainer;
	EvaluateParameters.TargetTags = TargetTagContainer;

	float Magnitude = 0;
	GetCapturedAttributeMagnitude(IntelligenceCapture, Spec, EvaluateParameters, Magnitude);

	Magnitude = FMath::Max(Magnitude, 0.0f);
	
	const ICombatInterface* CombatInterface =
		Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());

	if (CombatInterface)
	{
		return 80.0f + Magnitude * 1.5f + 5 * CombatInterface->GetPlayerLevel();
	}

	return 80.0f + Magnitude * 1.5f + 5;
}
