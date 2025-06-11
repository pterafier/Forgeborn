// Copyright Silvergrove Studio


#include "GAS/AbilitySystemComponentBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GAS/Abilities/GameplayAbilityBase.h"

void UAbilitySystemComponentBase::AbilityActorInfoSet()
{
	// bind callback for responding to effects when they're applied
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &ThisClass::EffectApplied); 
}

void UAbilitySystemComponentBase::AddStartupAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const UGameplayAbilityBase* Ability = Cast<UGameplayAbilityBase>(AbilitySpec.Ability))
		{
			GiveAbility(AbilitySpec);
		}
	}
}

void UAbilitySystemComponentBase::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass, float EffectLevel, const TMap<FGameplayTag, float>& SetByCallerMagnitudes)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC == nullptr) return;

	check(GameplayEffectClass);
	FGameplayEffectContextHandle EffectContextHandle = MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingSpec(GameplayEffectClass, EffectLevel, EffectContextHandle);

	for (const TPair<FGameplayTag, float>& Pair : SetByCallerMagnitudes)
	{
		EffectSpecHandle.Data->SetSetByCallerMagnitude(Pair.Key, Pair.Value);
	}

	ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data.Get(), TargetASC);
}

void UAbilitySystemComponentBase::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer; // Create a tag container
	EffectSpec.GetAllAssetTags(TagContainer); // Get all tags. Can use asset tags, granted tags, etc. Many container types to choose from but this is all tags that are applied to the effect
	
	EffectAssetTags.Broadcast(TagContainer);
	OnEffectAssetTagsChanged.Broadcast(TagContainer); // can bind to this in Blueprint to display UI elements based on a gameplay tag when an effect is applied
	
	for (FGameplayTag Tag : TagContainer)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Cyan, FString::Printf(TEXT("UAbilitySystemComponentBase: %s effect applied!"), *Tag.ToString()));
	}

	GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Cyan, FString::Printf(TEXT("UAbilitySystemComponentBase: effect applied!")));
}
