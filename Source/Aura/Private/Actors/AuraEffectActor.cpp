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

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> EffectToApplyClass)
{
	check(EffectToApplyClass);
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
			AbilitySystemComponent->MakeOutgoingSpec(EffectToApplyClass, 1.0f, EffectContextHandle);
		FActiveGameplayEffectHandle ActiveGameplayEffectHandle =
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

		const bool bInfinite = SpecHandle.Data->Def->DurationPolicy ==
			EGameplayEffectDurationType::Infinite;

		if (bInfinite &&
			InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap) 
		{
			ActiveGameplayEffects.Add(AbilitySystemComponent, ActiveGameplayEffectHandle);
		}
	}
}

void AAuraEffectActor::Overlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}

	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}

	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
}

void AAuraEffectActor::EndOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}

	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}

	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		UAbilitySystemComponent* AbilitySystemComponent =
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

		// The author of the course used a loop here to iterate through the map.
		// It can be avoided by simply swaping the types of keys and values when declaring the map.
		// Will it crush? - I believe it'll not. Why? - Considering that the lifetime of the objects is handled by the GC the pointer should be safe.
		// If I'm wrong, TWeakPtr should be used instead.
		if (AbilitySystemComponent &&
			ActiveGameplayEffects.Contains(AbilitySystemComponent))
		{
			FActiveGameplayEffectHandle ActiveGameplayEffectHandle =
				ActiveGameplayEffects[AbilitySystemComponent];

			AbilitySystemComponent->RemoveActiveGameplayEffect(ActiveGameplayEffectHandle, 1);
			ActiveGameplayEffects.Remove(AbilitySystemComponent);
		}
	}
}


