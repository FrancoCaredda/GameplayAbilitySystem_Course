#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraEffectActor.generated.h"

class UGameplayEffect;

UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:
	AAuraEffectActor();
protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> EffectToApplyClass);
protected:
	// Effects can be of types Instant, Duration and Infinite.
	// Instant effects apply changes to the BaseValue of attributes.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Effects")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;

	// Duration effects apply changes to the CurrentValue of attributes, and after the specified duration, the CurrentValue will roll back to the BaseValue.
	// Infinite effects apply changes to the CurrentValue of attributes, and if it's been discarded, the CurrentValue will roll back to the BaseValue.
	// However, if the period of an effect isn't null and the effect is of type Infinite or Duration, the effect will be applied to the BaseValue.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Effects")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;
};
