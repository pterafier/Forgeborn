// Copyright Silvergrove Studio

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

class UAttributeSet;

USTRUCT(BlueprintType)
struct FCharacterAttributes
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health = MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxStrength = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Strength = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HealthRegen = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HealthRegenRate = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PhysicalResistance = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MagicResistance = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WalkSpeed = 400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxWalkSpeed = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SprintSpeed = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxSprintSpeed = 800.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LookSpeed = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseMaxHealth = MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseHealth = Health;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseMaxStrength = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseStrength = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BasePhysicalResistance = PhysicalResistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseMagicResistance = MagicResistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseHealthRegenRate = HealthRegenRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseHealthRegen = HealthRegen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseLookSpeed = LookSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseMaxSprintSpeed = MaxSprintSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseSprintSpeed = SprintSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseMaxWalkSpeed = MaxWalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseWalkSpeed = WalkSpeed;
};

UCLASS()
class FORGEBORN_API ACharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ACharacterBase();

	/** Begin AbilitySystemInterface */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	/** End AbilitySystemInterface */
	virtual UAttributeSet* GetAttributeSet() const { return AttributeSet; };

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAttributeSet> AttributeSet;


};
