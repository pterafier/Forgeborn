// Copyright Silvergrove Studio

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "Actors/Interaction/WorkstationBase.h"
#include "Characters/CharacterBase.h"
#include "PlayerCharacter.generated.h"

class AItemBase;
class UNiagaraSystem;
class ADummyPawn;
class UGameplayAbility;
class APlayerStateBase;
class APlayerControllerBase;
class UCameraComponent;
class USpringArmComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerLookAt, AActor*, Actor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHammerAnimComplete, int, ItemIdToCraft);

UENUM(BlueprintType)
enum class EItems : uint8
{
	None				UMETA(DisplayName = "None"),
	IronSword			UMETA(DisplayName = "Iron Sword"),
	TitaniumSword		UMETA(DisplayName = "Titanium Sword"),
	SteelSword			UMETA(DisplayName = "Steel Sword"),
	ViridiumSword		UMETA(DisplayName = "Viridium Sword"),
	IronPickaxe			UMETA(DisplayName = "Iron Pickaxe"),
	TitaniumPickaxe		UMETA(DisplayName = "Titanium Pickaxe"),
	SteelPickaxe		UMETA(DisplayName = "Steel Pickaxe"),
	ViridiumPickaxe		UMETA(DisplayName = "Viridium Pickaxe"),
	IronAxe				UMETA(DisplayName = "Iron Axe"),
	TitaniumAxe			UMETA(DisplayName = "Titanium Axe"),
	SteelAxe			UMETA(DisplayName = "Steel Axe"),
	ViridiumAxe			UMETA(DisplayName = "Viridium Axe"),
	IronIngot			UMETA(DisplayName = "Iron Ingot"),
	TitaniumIngot		UMETA(DisplayName = "Titanium Ingot"),
	SteelIngot			UMETA(DisplayName = "Steel Ingot"),
	ViridiumIngot		UMETA(DisplayName = "Viridium Ingot"),
	IronNails			UMETA(DisplayName = "Iron Nails"),
	TitaniumNails		UMETA(DisplayName = "Titanium Nails"),
	SteelNails			UMETA(DisplayName = "Steel Nails"),
	ViridiumNails		UMETA(DisplayName = "Viridium Nails"),
	IronHorseshoes		UMETA(DisplayName = "Iron Horseshoes"),
	TitaniumHorseshoes	UMETA(DisplayName = "Titanium Horseshoes"),
	SteelHorseshoes		UMETA(DisplayName = "Steel Horseshoes"),
	ViridiumHorseshoes	UMETA(DisplayName = "Viridium Horseshoes"),
	IronOre				UMETA(DisplayName = "Iron Ore"),
	TitaniumOre			UMETA(DisplayName = "Titanium Ore"),
	SteelOre			UMETA(DisplayName = "Steel Ore"),
	ViridiumOre			UMETA(DisplayName = "Viridium Ore"),
};

UENUM(BlueprintType)
enum class EOreTypes : uint8
{
	None		UMETA(DisplayName = "None"),
	Iron		UMETA(DisplayName = "Iron"),
	Titanium	UMETA(DisplayName = "Titanium"),
	Steel		UMETA(DisplayName = "Steel"),
	Viridium	UMETA(DisplayName = "Viridium")
};

UENUM(BlueprintType)
enum class EItemQuality : uint8
{
	None		UMETA(DisplayName = "None"),
	Poor		UMETA(DisplayName = "Poor"),
	Fair		UMETA(DisplayName = "Fair"),
	Good		UMETA(DisplayName = "Good"),
	Excellent	UMETA(DisplayName = "Excellent"),
	Legendary	UMETA(DisplayName = "Legendary"),
};

USTRUCT(BlueprintType)
struct FItem : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Id = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description = FText();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool TransformsAfterReachingTargetTemperature = false;

	// Can also be used to dictate when an item is at the correct temperature for shaping
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TargetTransformationTemperature = -1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool TakesDamage = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageTemperature = 2000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxTemperature = 5000.f;

	// The data table row name of the item this transforms into after reaching its target temperature, if at all
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName TransformsInto_DataTableRowName = FName();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool RequiresQuenching = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool RequiresSharpening = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemQuality FinalQuality = EItemQuality::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxQuality = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItems Type = EItems::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EOreTypes Ore = EOreTypes::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxStackSize = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isValid = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* Mesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraSystem* EnchantmentNiagaraSystem = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FQTEConfig> AnvilQuickTimeConfig;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FQTEConfig> GrindstoneQuickTimeConfig;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EForgeState ForgeState = EForgeState::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int BaseQuality = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int BaseValue = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UGameplayEffect* EnchantmentEffect = nullptr;
};

USTRUCT(BlueprintType)
struct FInventorySlot : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItem Item = FItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Quantity = -1;
};

/**
 * 
 */
UCLASS()
class FORGEBORN_API APlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	APlayerCharacter();
	
/*
 * Public Variables
 */
	// The main camera for the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UCameraComponent> Camera;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> HammerMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USceneComponent> HoldLocation;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References")
	TObjectPtr<APlayerControllerBase> PlayerControllerBase;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References")
	TObjectPtr<APlayerStateBase> PlayerStateBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TObjectPtr<AItemBase> EquippedItem;

	// An array of abilities to grant to the player when the game starts
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float InteractDistance;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interaction")
	TObjectPtr<AActor> LookAtActor;

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnPlayerLookAt OnPlayerLookAt;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Minigame")
	FOnHammerAnimComplete OnHammerAnimComplete;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Possession")
	TSubclassOf<ADummyPawn> TempPawnToPossess;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Possession")
	TObjectPtr<ADummyPawn> PossessedDummyPawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interaction")
	bool hasWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interaction")
	bool AtGrindingWheel;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interaction")
	bool AtAnvil;

/*
 * Public Methods
 */
	// Called when the character gets possessed by the controller
	virtual void PossessedBy(AController* NewController) override;

	UFUNCTION(BlueprintCallable, Category = "Save")
	FCharacterAttributes GetAttributesSaveData();

	UFUNCTION(BlueprintCallable)
	void Interact();

	UFUNCTION(BlueprintCallable)
	void AdjustCameraZoom();

	UFUNCTION(BlueprintCallable)
	void SetEquippedItem(AItemBase* NewItem);

	UFUNCTION()
	void PossessAIController();

	UFUNCTION()
	void PossessPlayerController(UCommonActivatableWidget* WidgetToRemove);

	UFUNCTION(BlueprintCallable)
	void HideHammer(bool Hide);

protected:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	void AddStartupAbilities();

private:
	virtual void InitAbilityActorInfo() override;

	// Function run on tick to detect interactable objects
	void InteractTrace();

	void RotateCameraBackToCenter();

	// Helper function to clean up Interact Trace
	AActor* PerformInteractLineTrace();

	// Organizational method to reduce lines of code on PossessedBy. Just sets references to necessary actors in the game
	void SetPlayerReferences(AController* NewController);

	FRotator StartupCameraRotation;
	FTimerHandle CameraRotationTimerHandle;
};
