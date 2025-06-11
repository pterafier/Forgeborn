// Copyright Silvergrove Studio

/*
 * 
 */

#pragma once

#include "CoreMinimal.h"
#include "Characters/PlayerCharacter.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryChanged, int, SlotIndex);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FORGEBORN_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TObjectPtr<UInventoryComponent> OwnerActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FItem EmptyItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<FInventorySlot> Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FItem EquippedItem;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryChanged OnInventoryChanged;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void Add(const FItem& Item, int QuantityToAdd);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void Remove(int SlotIndex);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void MoveItemToNewSlot(int FromSlot, int ToSlot, int QuantityToMove, UInventoryComponent* FromInventory, UInventoryComponent* ToInventory);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool FindEmptySlot(int32& OutSlotIndex);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FInventorySlot FindItemById(int32 Id, bool& ItemFound, int& OutSlotIndex);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 FindFirstIndexContainingItem(FInventorySlot& Item, bool& IndexFound);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool FindItemAtIndex(int32 Index, FInventorySlot& OutItem);

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Inventory")
	bool CanItemsStack(FInventorySlot& ItemInInventory, FInventorySlot& NewItem, int& Remainder) const;

protected:
	virtual void BeginPlay() override;

		
};
