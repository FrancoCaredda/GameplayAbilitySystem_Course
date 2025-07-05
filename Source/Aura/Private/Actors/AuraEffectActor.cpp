#include "Actors/AuraEffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemBlueprintLibrary.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("Root"));
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, const FGameplayEffectConfig& EffectConfig)
{
	check(EffectConfig.GameplayEffectClass);
	UAbilitySystemComponent* AbilitySystemComponent =
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	if (AbilitySystemComponent)
	{
		// To apply a gameplay effect, a context for it should be created.
		// FGameplayEffectContext is a data structure that describes who has initiated the effect and some other data.
		// FGameplayEffectContext is created via UAbilitySystemComponent::MakeEffectContext(), and this function returns a handle to an instance of the structure
		FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);
		// However it's not enough to apply a gameplay effect.
		// Seen that UGameplayEffect is a subclass of UObject it's very heavy to instantiate.
		// FGameplayEffectSpecHandle is a lightweight structure that describes the effect itself (modifiers, levels, magnitudes, etc.).
		// IMPORTANT: UGameplayEffect shouldn't contain any custom logic. It should be extended via blueprints and serve as data blueprint classes.
		FGameplayEffectSpecHandle SpecHandle =
			AbilitySystemComponent->MakeOutgoingSpec(EffectConfig.GameplayEffectClass, 1.0f, EffectContextHandle);
		FActiveGameplayEffectHandle ActiveGameplayEffectHandle =
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

		const bool bInfinite = SpecHandle.Data->Def->DurationPolicy ==
			EGameplayEffectDurationType::Infinite;

		if (bInfinite &&
			EffectConfig.RemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap) 
		{
			ActiveGameplayEffects.Add(AbilitySystemComponent,
				ActiveGameplayEffectHandle);
		}
	}
}

void AAuraEffectActor::Overlap(AActor* TargetActor)
{
	for (const FGameplayEffectConfig& gameplayEffectConfig : EffectConfigs)
	{
		if (gameplayEffectConfig.ApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
		{
			ApplyEffectToTarget(TargetActor, gameplayEffectConfig);
		}
	}
}

void AAuraEffectActor::EndOverlap(AActor* TargetActor)
{
	for (const FGameplayEffectConfig& gameplayEffectConfig : EffectConfigs)
	{
		if (gameplayEffectConfig.ApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
		{
			ApplyEffectToTarget(TargetActor, gameplayEffectConfig);
		}
	}

	UAbilitySystemComponent* AbilitySystemComponent =
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	if (AbilitySystemComponent &&
		ActiveGameplayEffects.Contains(AbilitySystemComponent))
	{
		TArray<FActiveGameplayEffectHandle> ActiveGameplayEffectHandles;
		ActiveGameplayEffects.MultiFind(AbilitySystemComponent, ActiveGameplayEffectHandles);

		for (const FActiveGameplayEffectHandle& ActiveGameplayEffectHandle : ActiveGameplayEffectHandles)
		{
			AbilitySystemComponent->RemoveActiveGameplayEffect(ActiveGameplayEffectHandle, 1);
		}
		
		ActiveGameplayEffects.Remove(AbilitySystemComponent);
	}
}