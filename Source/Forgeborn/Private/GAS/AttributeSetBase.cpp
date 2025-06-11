// Copyright Silvergrove Studio


#include "GAS/AttributeSetBase.h"

#include "GameplayEffectExtension.h"

UAttributeSetBase::UAttributeSetBase()
{
	// Attributes initialized via gameplay effects on the player character class
}

void UAttributeSetBase::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	// Vital Attributes
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	
	// Primary Attributes
	if (Attribute == GetStrengthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxStrength());
	}

	// Movement Attributes
	if (Attribute == GetWalkSpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxWalkSpeed());
	}
	if (Attribute == GetSprintSpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxSprintSpeed());
	}
}

void UAttributeSetBase::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// Vital Attributes
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}

	// Primary Attributes
	if (Data.EvaluatedData.Attribute == GetStrengthAttribute())
	{
		SetStrength(FMath::Clamp(GetStrength(), 0.f, GetMaxStrength()));
	}

	// Movement Attributes
	if (Data.EvaluatedData.Attribute == GetWalkSpeedAttribute())
	{
		SetWalkSpeed(FMath::Clamp(GetWalkSpeed(), 0.f, GetMaxWalkSpeed()));
	}
	if (Data.EvaluatedData.Attribute == GetSprintSpeedAttribute())
	{
		SetSprintSpeed(FMath::Clamp(GetSprintSpeed(), 0.f, GetMaxSprintSpeed()));
	}
}
