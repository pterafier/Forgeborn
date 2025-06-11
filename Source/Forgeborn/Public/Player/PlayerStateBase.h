// Copyright Silvergrove Studio

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "PlayerStateBase.generated.h"


struct FOnAttributeChangeData;
struct FCharacterAttributes;
class UAttributeSet;
class UGameplayEffect;
class APlayerControllerBase;
class APlayerCharacter;

USTRUCT(BlueprintType)
struct FPlayerSave : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	FTransform PlayerTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	FRotator PlayerControlRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float PlayerHealth;
	
	FPlayerSave() :
		PlayerTransform(FTransform(FRotator::ZeroRotator, FVector::ZeroVector, FVector(1.0f))),
		PlayerControlRotation(FRotator::ZeroRotator),
		PlayerHealth(-1.f)
	{}
};

/**
 * 
 */
UCLASS()
class FORGEBORN_API APlayerStateBase : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	APlayerStateBase();

/*
 * Public Variables
 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References")
	TObjectPtr<APlayerCharacter> PlayerCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References")
	TObjectPtr<APlayerControllerBase> PlayerControllerBase;

	// If true, retrieve the save game object and set the player's state based on the data there
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Save")
	bool LoadPlayerDataAtBeginPlay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> InitializeAttributesEffect;

/*
 * Public Methods
 */
	
	UFUNCTION(BlueprintCallable, Category = "Save")
	FPlayerSave GetPlayerSaveData();

	UFUNCTION(BlueprintCallable, Category = "Save")
	void SetPlayerStateFromSaveData();

	UFUNCTION()
	void OnSaveGameObjectLoaded_Callback();

	//* Begin AbilitySystemInterface */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//* End AbilitySystemInterface */
	
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

private:
	void InitializeAttributes(const FCharacterAttributes& PlayerAttributes);
	void SetReferences();
	void BindAttributeDelegates();
/**
 * Delegate Callbacks
 */
	void OnWalkSpeedChanged_Callback(const FOnAttributeChangeData& Data);
	void OnLookSpeedChanged_Callback(const FOnAttributeChangeData& Data);

	// Used to ensure we don't bind to OnSaveGameLoaded delegate multiple times
	bool BoundToGameLoadedDelegate; 
	
};
