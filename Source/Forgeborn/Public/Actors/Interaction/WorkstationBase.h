// Copyright Silvergrove Studio

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractInterface.h"
#include "UI/Widgets/HUD/Minigame/MinigameWidgetBase.h"
#include "WorkstationBase.generated.h"


class UMinigameWidgetBase;
class UArrowComponent;

namespace EPathFollowingResult
{
	enum Type : int;
}

struct FAIRequestID;
class UWidgetComponent;
class UCommonActivatableWidget;
class UActivatableWidgetBase;
class APlayerCharacter;
class UCineCameraComponent;
class USphereComponent;
class UStaticMeshComponent;

USTRUCT(BlueprintType)
struct FWeaponCrafting : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FQTEConfig> QuickTimeConfig;
};

UENUM(BlueprintType)
enum class EWorkstationType : uint8
{
	None		UMETA(DisplayName = "None"),
	Bellows		UMETA(DisplayName = "Bellows"),
	Forge		UMETA(DisplayName = "Forge"),
	Anvil		UMETA(DisplayName = "Anvil"),
	Quench		UMETA(DisplayName = "Quench"),
	Grindstone	UMETA(DisplayName = "Grindstone"),
	OreChest	UMETA(DisplayName = "OreChest"),
	IngotChest	UMETA(DisplayName = "IngotChest"),
	CoalChest	UMETA(DisplayName = "CoalChest"),
};

UENUM(BlueprintType)
enum class EWorkstationAction : uint8
{
	None		UMETA(DisplayName = "None"),
	Add			UMETA(DisplayName = "Add"),
	Remove		UMETA(DisplayName = "Remove"),
	Modify		UMETA(DisplayName = "Modify")
};

// TODO: Move this to an item or craftable class
// also add temperature, sharpness, quality
UENUM(BlueprintType)
enum class EForgeState : uint8
{
	None				UMETA(DisplayName = "None"),
	Cool				UMETA(DisplayName = "Cool"),
	Hot					UMETA(DisplayName = "Hot"),
	Cooling				UMETA(DisplayName = "Cooling"),
	Heating				UMETA(DisplayName = "Heating"),
	Ingot				UMETA(DisplayName = "Ingot"),
	ShapingInProgress	UMETA(DisplayName = "Shaping In Progress"),
	Shaped				UMETA(DisplayName = "Shaped"),
	Quenched			UMETA(DisplayName = "Quenched"),
	Tempered			UMETA(DisplayName = "Tempered"),
	Sharpened			UMETA(DisplayName = "Sharpened"),
};

UENUM(BlueprintType)
enum class EQuenchResult : uint8
{
	None				UMETA(DisplayName = "None"),
	Perfect				UMETA(DisplayName = "Perfect"),
	TooHot				UMETA(DisplayName = "TooHot"),
	TooCool				UMETA(DisplayName = "TooCool"),
};

UCLASS()
class FORGEBORN_API AWorkstationBase : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	AWorkstationBase();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USceneComponent> ActorRoot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USphereComponent> SphereCollision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USphereComponent> AnimationTargetLocation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UCineCameraComponent> WorkstationCamera;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UArrowComponent> Arrow;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<APlayerCharacter> PlayerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties")
	EWorkstationType WorkstationType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties")
	bool hasMinigame;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	bool isAcceptingItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	TArray<int> AcceptableItemIds;

	// The action the player will perform on this workstation when it's interacted with
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWorkstationAction WorkstationAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CameraBlendExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UActivatableWidgetBase> MinigameWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCommonActivatableWidget* MinigameWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> PopupWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isLocked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ShouldLock;

	virtual void Interact_Implementation() override;
	virtual void LookAt_Implementation() override;

	UFUNCTION(BlueprintCallable)
	void HidePopupWidget();

	UFUNCTION(BlueprintCallable)
	void ShowPopupWidget();

	UFUNCTION(BlueprintCallable)
	void SetWorkstationLocked(bool Locked);

	UFUNCTION(BlueprintCallable)
	void PlayerExitWorkstation();

	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayerExitWorkstation();

	UFUNCTION(BlueprintImplementableEvent)
	void OnMinigameWidgetActivated();

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnSphereBeginOverlap_InteractSphere(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnSphereEndOverlap_InteractSphere(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	UFUNCTION()
	void OnSphereBeginOverlap_AnimationSphere(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnSphereEndOverlap_AnimationSphere(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	UFUNCTION()
	void OnAIMoveToComplete(FAIRequestID RequestID,  EPathFollowingResult::Type Result);

	bool isInteractable;

	void SnapToWorkstation();

	void BlendViewToWorkstation();

	void SetPlayerAtWorkstation();
};
