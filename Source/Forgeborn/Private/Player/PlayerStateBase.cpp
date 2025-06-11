// Copyright Silvergrove Studio


#include "Player/PlayerStateBase.h"

#include "AbilitySystemComponent.h"
#include "Characters/PlayerCharacter.h"
#include "Game/Systems/Save/SaveGameObject.h"
#include "Game/Systems/Save/SaveGameSubsystem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/AbilitySystemComponentBase.h"
#include "GAS/AttributeSetBase.h"
#include "Kismet/GameplayStatics.h"

APlayerStateBase::APlayerStateBase()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponentBase>("Ability System Component");

	AttributeSet = CreateDefaultSubobject<UAttributeSetBase>("Attribute Set");
	
	LoadPlayerDataAtBeginPlay = true;
	BoundToGameLoadedDelegate = false;
}

FPlayerSave APlayerStateBase::GetPlayerSaveData()
{
	FPlayerSave PlayerData;
	PlayerData.PlayerTransform = PlayerCharacter->GetTransform();
	PlayerData.PlayerControlRotation = PlayerCharacter->GetControlRotation();

	return PlayerData;
}

void APlayerStateBase::SetPlayerStateFromSaveData()
{
	USaveGameSubsystem* SaveSubsystem = USaveGameSubsystem::Get(this);
	USaveGameObject* SaveGameObject = ISaveGameInterface::Execute_GetSaveGameData(SaveSubsystem);

	if (SaveSubsystem->DoesSaveExist)
	{
		if (SaveSubsystem->SaveObjectInitialized)
		{
			FPlayerSave PlayerData = SaveGameObject->PlayerData;
			PlayerCharacter->SetActorTransform(PlayerData.PlayerTransform);
			PlayerControllerBase->SetControlRotation(PlayerData.PlayerControlRotation);
			
			InitializeAttributes(SaveGameObject->PlayerAttributes);
			PlayerControllerBase->SetControlsFromSaveData();
		}
		else
		{
			if (BoundToGameLoadedDelegate) return;
			SaveSubsystem->OnSaveGameObjectLoaded.AddDynamic(this, &ThisClass::OnSaveGameObjectLoaded_Callback);
			BoundToGameLoadedDelegate = true;
		}
	}
	else
	{
		FCharacterAttributes NewPlayerAttributes;
		InitializeAttributes(NewPlayerAttributes);
	}
}

void APlayerStateBase::OnSaveGameObjectLoaded_Callback()
{
	SetPlayerStateFromSaveData();
}

UAbilitySystemComponent* APlayerStateBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void APlayerStateBase::BeginPlay()
{
	Super::BeginPlay();

	SetReferences();
	BindAttributeDelegates();
	if (LoadPlayerDataAtBeginPlay)
	{
		SetPlayerStateFromSaveData();
	}
}

void APlayerStateBase::InitializeAttributes(const FCharacterAttributes& PlayerAttributes)
{
	check(AbilitySystemComponent);
	check(InitializeAttributesEffect);

	FGameplayEffectSpecHandle EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
		InitializeAttributesEffect,
		1.0f,
		AbilitySystemComponent->MakeEffectContext()
	);
	
	FGameplayEffectSpec* EffectSpec = EffectSpecHandle.Data.Get();
	EffectSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attributes.MaxHealth")), PlayerAttributes.MaxHealth);
	EffectSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attributes.Health")), PlayerAttributes.Health);
	EffectSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attributes.Strength")), PlayerAttributes.Strength);
	EffectSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attributes.MaxStrength")), PlayerAttributes.MaxStrength);
	EffectSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attributes.HealthRegen")), PlayerAttributes.HealthRegen);
	EffectSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attributes.HealthRegenRate")), PlayerAttributes.HealthRegenRate);
	EffectSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attributes.PhysicalResistance")), PlayerAttributes.PhysicalResistance);
	EffectSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attributes.MagicResistance")), PlayerAttributes.MagicResistance);
	EffectSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attributes.WalkSpeed")), PlayerAttributes.WalkSpeed);
	EffectSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attributes.MaxWalkSpeed")), PlayerAttributes.MaxWalkSpeed);
	EffectSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attributes.SprintSpeed")), PlayerAttributes.SprintSpeed);
	EffectSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attributes.MaxSprintSpeed")), PlayerAttributes.MaxSprintSpeed);
	EffectSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attributes.LookSpeed")), PlayerAttributes.LookSpeed);
	EffectSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attributes.BaseMaxHealth")), PlayerAttributes.BaseMaxHealth);
	EffectSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attributes.BaseHealth")), PlayerAttributes.BaseHealth);
	EffectSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attributes.BaseMaxStrength")), PlayerAttributes.BaseMaxStrength);
	EffectSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attributes.BaseStrength")), PlayerAttributes.BaseStrength);
	EffectSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attributes.BasePhysicalResistance")), PlayerAttributes.BasePhysicalResistance);
	EffectSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attributes.BaseMagicResistance")), PlayerAttributes.BaseMagicResistance);
	EffectSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attributes.BaseHealthRegenRate")), PlayerAttributes.BaseHealthRegenRate);
	EffectSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attributes.BaseHealthRegen")), PlayerAttributes.BaseHealthRegen);
	EffectSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attributes.BaseLookSpeed")), PlayerAttributes.BaseLookSpeed);
	EffectSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attributes.BaseMaxSprintSpeed")), PlayerAttributes.BaseMaxSprintSpeed);
	EffectSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attributes.BaseSprintSpeed")), PlayerAttributes.BaseSprintSpeed);
	EffectSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attributes.BaseMaxWalkSpeed")), PlayerAttributes.BaseMaxWalkSpeed);
	EffectSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attributes.BaseWalkSpeed")), PlayerAttributes.BaseWalkSpeed);
	
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpec);
}

void APlayerStateBase::SetReferences()
{
	PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	PlayerControllerBase = Cast<APlayerControllerBase>(UGameplayStatics::GetPlayerController(this, 0));
}

void APlayerStateBase::BindAttributeDelegates()
{
	check(AbilitySystemComponent)
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UAttributeSetBase::GetWalkSpeedAttribute()).AddUObject(this, &ThisClass::OnWalkSpeedChanged_Callback);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UAttributeSetBase::GetLookSpeedAttribute()).AddUObject(this, &ThisClass::OnLookSpeedChanged_Callback);
}

void APlayerStateBase::OnWalkSpeedChanged_Callback(const FOnAttributeChangeData& Data)
{
	PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = Data.NewValue;
}

void APlayerStateBase::OnLookSpeedChanged_Callback(const FOnAttributeChangeData& Data)
{
	PlayerControllerBase->BaseTurnRate = Data.NewValue;
}
