#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "AuraEffectActor.generated.h"

class UAbilitySystemComponent;
class UGameplayEffect;

UENUM(BlueprintType)
enum class EEffectApplicationPolicy : uint8
{
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DoNotApply
};

UENUM(BlueprintType)
enum class EEffectRemovalPolicy : uint8
{
	RemoveOnEndOverlap,
	DoNotRemove
};

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

	UFUNCTION(BlueprintCallable)
	void Overlap(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void EndOverlap(AActor* TargetActor);
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Effects")
	bool bDestroyOnEffectRemoval = false;
	
	// Effects can be of types Instant, Duration and Infinite.
	// Instant effects apply changes to the BaseValue of attributes.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Effects")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Effects")
	EEffectApplicationPolicy InstantEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
	
	// Duration effects apply changes to the CurrentValue of attributes, and after the specified duration, the CurrentValue will roll back to the BaseValue.
	// Infinite effects apply changes to the CurrentValue of attributes, and if it's been discarded, the CurrentValue will roll back to the BaseValue.
	// However, if the period of an effect isn't null and the effect is of type Infinite or Duration, the effect will be applied to the BaseValue.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Effects")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Effects")
	EEffectApplicationPolicy DurationEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Effects")
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Effects")
	EEffectApplicationPolicy InfiniteEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Effects")
	EEffectRemovalPolicy InfiniteEffectRemovalPolicy = EEffectRemovalPolicy::RemoveOnEndOverlap;
private:
	TMap<UAbilitySystemComponent*, FActiveGameplayEffectHandle> ActiveGameplayEffects;
};
