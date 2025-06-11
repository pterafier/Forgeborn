// Copyright Silvergrove Studio

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "Interfaces/InteractInterface.h"
#include "NPCBase.generated.h"

class UWidgetComponent;
enum class EItemQuality : uint8;
enum class EItems : uint8;
class USphereComponent;

UENUM(BlueprintType)
enum class ECustomerType : uint8
{
	None			UMETA(DisplayName = "None"),
	Merchant		UMETA(DisplayName = "Merchant"),
	Adventurer		UMETA(DisplayName = "Adventurer"),
	Townsperson		UMETA(DisplayName = "Townsperson"),
};

/**
 * 
 */
UCLASS()
class FORGEBORN_API ANPCBase : public ACharacterBase, public IInteractInterface
{
	GENERATED_BODY()

public:
	ANPCBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> PopupWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (ExposeOnSpawn = "true"))
	TArray<int> DesiredItemIds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (ExposeOnSpawn = "true"))
	int OrderSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (ExposeOnSpawn = "true"))
	ECustomerType CustomerType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (ExposeOnSpawn = "true"))
	int GoldBonus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (ExposeOnSpawn = "true"))
	float WaitTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (ExposeOnSpawn = "true"))
	int OrderDuration;

	UFUNCTION(BlueprintCallable)
	void HidePopupWidget();

	UFUNCTION(BlueprintCallable)
	void ShowPopupWidget();

	virtual void Interact_Implementation() override;
	virtual void LookAt_Implementation() override;

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
	
};
