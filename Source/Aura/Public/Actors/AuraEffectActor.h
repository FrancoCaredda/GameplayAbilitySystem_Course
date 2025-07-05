#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/Map.h"
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

USTRUCT(BlueprintType)
struct FGameplayEffectConfig
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UGameplayEffect> GameplayEffectClass;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EEffectApplicationPolicy ApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EEffectRemovalPolicy RemovalPolicy = EEffectRemovalPolicy::DoNotRemove;
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
	void ApplyEffectToTarget(AActor* TargetActor, const FGameplayEffectConfig& EffectConfig);

	UFUNCTION(BlueprintCallable)
	void Overlap(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void EndOverlap(AActor* TargetActor);
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Effects")
	bool bDestroyOnEffectRemoval = false;
	
	// Effects can be of types Instant, Duration and Infinite.
	// Instant effects apply changes to the BaseValue of attributes.
	// Duration effects apply changes to the CurrentValue of attributes, and after the specified duration, the CurrentValue will roll back to the BaseValue.
	// Infinite effects apply changes to the CurrentValue of attributes, and if it's been discarded, the CurrentValue will roll back to the BaseValue.
	// However, if the period of an effect isn't null and the effect is of type Infinite or Duration, the effect will be applied to the BaseValue.

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Effects")
	TArray<FGameplayEffectConfig> EffectConfigs;
private:
	// Special Quest: Multiple active effects
	TMultiMap<TWeakObjectPtr<UAbilitySystemComponent>, FActiveGameplayEffectHandle> ActiveGameplayEffects;
};
