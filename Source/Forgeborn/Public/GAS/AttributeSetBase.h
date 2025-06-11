// Copyright Silvergrove Studio

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "AttributeSetBase.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class FORGEBORN_API UAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()

public:
    UAttributeSetBase();

/**
* Vital Attributes
*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vital Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, Health);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vital Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, MaxHealth);

/**
 * Primary Attributes
 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Primary Attributes")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, Strength);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Primary Attributes")
	FGameplayAttributeData MaxStrength;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, MaxStrength);

/**
 * Secondary Attributes
 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Secondary Attributes")
	FGameplayAttributeData HealthRegen;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, HealthRegen);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Secondary Attributes")
	FGameplayAttributeData HealthRegenRate;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, HealthRegenRate);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Secondary Attributes")
	FGameplayAttributeData PhysicalResistance;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, PhysicalResistance);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Secondary Attributes")
	FGameplayAttributeData MagicResistance;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, MagicResistance);

/**
 * Movement Attributes
 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Attributes")
	FGameplayAttributeData WalkSpeed;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, WalkSpeed);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Attributes")
	FGameplayAttributeData MaxWalkSpeed;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, MaxWalkSpeed);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Attributes")
	FGameplayAttributeData SprintSpeed;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, SprintSpeed);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Attributes")
	FGameplayAttributeData MaxSprintSpeed;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, MaxSprintSpeed);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Attributes")
	FGameplayAttributeData LookSpeed;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, LookSpeed);

/**
 * Base Attributes
 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Attributes")
	FGameplayAttributeData BaseWalkSpeed;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, BaseWalkSpeed);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Attributes")
	FGameplayAttributeData BaseMaxWalkSpeed;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, BaseMaxWalkSpeed);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Attributes")
	FGameplayAttributeData BaseSprintSpeed;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, BaseSprintSpeed);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Attributes")
	FGameplayAttributeData BaseMaxSprintSpeed;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, BaseMaxSprintSpeed);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Attributes")
	FGameplayAttributeData BaseLookSpeed;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, BaseLookSpeed);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Attributes")
	FGameplayAttributeData BaseHealthRegen;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, BaseHealthRegen);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Attributes")
	FGameplayAttributeData BaseHealthRegenRate;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, BaseHealthRegenRate);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Attributes")
	FGameplayAttributeData BasePhysicalResistance;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, BasePhysicalResistance);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Attributes")
	FGameplayAttributeData BaseMagicResistance;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, BaseMagicResistance);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Attributes")
	FGameplayAttributeData BaseStrength;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, BaseStrength);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Attributes")
	FGameplayAttributeData BaseMaxStrength;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, BaseMaxStrength);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Attributes")
	FGameplayAttributeData BaseHealth;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, BaseHealth);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Attributes")
	FGameplayAttributeData BaseMaxHealth;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, BaseMaxHealth);

public:
	// Used to clamp internal values of attributes
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

	// Used to clamp attribute values for display purposes
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
};
