// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMCMaxMana.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UMMCMaxMana : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
public:
	UMMCMaxMana();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const;
private:
	FGameplayEffectAttributeCaptureDefinition IntelligenceCapture;
};
