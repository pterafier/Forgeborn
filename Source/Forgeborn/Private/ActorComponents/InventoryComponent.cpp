// Copyright Silvergrove Studio


#include "ActorComponents/InventoryComponent.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UInventoryComponent::Add(const FItem& InItem, int QuantityToAdd)
{
	int RemainingQuantity = QuantityToAdd;

	// Try to fill existing stacks
	for (FInventorySlot& ItemStack : Inventory)
	{
		if (ItemStack.Item.Id == InItem.Id && ItemStack.Quantity < InItem.MaxStackSize)
		{
			int32 SpaceInStack = InItem.MaxStackSize - ItemStack.Quantity;
			int32 AmountToAdd = FMath::Min(RemainingQuantity, SpaceInStack);
			ItemStack.Quantity += AmountToAdd;
			RemainingQuantity -= AmountToAdd;

			GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Yellow, "Stacking criteria met, attempting to stack item");

			if (RemainingQuantity <= 0)
			{
				return; // All items added, exit early
			}
		}
	}

	// If there are still items left to add, create new stacks
	while (RemainingQuantity > 0)
	{
		FInventorySlot NewStack;
		NewStack.Item = InItem;

		int EmptySlotIndex;
		FindEmptySlot(EmptySlotIndex);
		int32 AmountToAdd = FMath::Min(RemainingQuantity, InItem.MaxStackSize);
		NewStack.Quantity = AmountToAdd;
		Inventory[EmptySlotIndex] = NewStack;

		RemainingQuantity -= AmountToAdd;

		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Yellow, FString::Printf(TEXT("Creating new stack for item: Slot %i"), EmptySlotIndex));
	}
}

void UInventoryComponent::Remove(int SlotIndex)
{
	Inventory[SlotIndex].Item = EmptyItem;
}

void UInventoryComponent::MoveItemToNewSlot(int FromSlot, int ToSlot, int QuantityToMove, UInventoryComponent* FromInventory, UInventoryComponent* ToInventory)
{
	if (FromSlot == ToSlot && FromInventory == ToInventory) return; // picked up and dropped in same slot = do nothing

	// Store data about the To slot
	FInventorySlot SavedItemInToSlot;
	bool isItemInToSlot = ToInventory->FindItemAtIndex(ToSlot, SavedItemInToSlot);
	int ToSlotItemId = SavedItemInToSlot.Item.Id;

	// Store data about the From slot - there will always be an item in the From slot
	FInventorySlot SavedItemInFromSlot;
	FromInventory->FindItemAtIndex(FromSlot, SavedItemInFromSlot);
	int FromSlotItemId = SavedItemInFromSlot.Item.Id;

	int StackRemainder = -1;
	if (isItemInToSlot)
	{
		bool ShouldStack = CanItemsStack(SavedItemInToSlot, SavedItemInFromSlot, StackRemainder);
		if (ShouldStack)
		{
			// Max out the stack in the To slot and replace the From slot quantity with the remainder
			if (StackRemainder > 0)
			{
				ToInventory->Inventory[ToSlot].Quantity += SavedItemInFromSlot.Quantity - StackRemainder;
				FromInventory->Inventory[FromSlot].Quantity = StackRemainder;

				ToInventory->OnInventoryChanged.Broadcast(ToSlot);
				FromInventory->OnInventoryChanged.Broadcast(FromSlot);
			}
			else
			{
				// To slot can receive the full stack, so stack, then replace the From slot with an empty item
				ToInventory->Inventory[ToSlot].Quantity += SavedItemInFromSlot.Quantity;
				FromInventory->Inventory[FromSlot] = FInventorySlot();

				ToInventory->OnInventoryChanged.Broadcast(ToSlot);
				FromInventory->OnInventoryChanged.Broadcast(FromSlot);
			}
		}
		else
		{
			// Swap items
		}
	}
}

bool UInventoryComponent::FindEmptySlot(int32& OutSlotIndex)
{
	bool EmptySlotFound = false;
	
	int SlotIndex = 0;
	for (FInventorySlot& Slot : Inventory)
	{
		if (Slot.Item.isValid)
		{
			EmptySlotFound = false;
			OutSlotIndex = -1;
		}
		else
		{
			EmptySlotFound = true;
			OutSlotIndex = SlotIndex;
			break;
		}
		SlotIndex++;
	}
	return EmptySlotFound;
}

FInventorySlot UInventoryComponent::FindItemById(int32 Id, bool& ItemFound, int& OutSlotIndex)
{
	int SlotIndex = 0;
	for (FInventorySlot& Slot : Inventory)
	{
		if (Slot.Item.Id == Id)
		{
			ItemFound = true;
			OutSlotIndex = SlotIndex;
			return Slot;
		}
		else
		{
			ItemFound = false;
			OutSlotIndex = -1;
		}
		SlotIndex++;
	}
	return FInventorySlot();
}

int32 UInventoryComponent::FindFirstIndexContainingItem(FInventorySlot& InItem, bool& IndexFound)
{
	int SlotIndex = 0;
	for (FInventorySlot& Slot : Inventory)
	{
		if (Slot.Item.Id == InItem.Item.Id)
		{
			InItem = Slot;
			IndexFound = true;
			return SlotIndex;
		}
		else
		{
			InItem = FInventorySlot();
			IndexFound = false;
		}
		SlotIndex++;
	}
	return -1;
}

bool UInventoryComponent::FindItemAtIndex(int32 Index, FInventorySlot& OutItem)
{
	if (Inventory[Index].Item.isValid)
	{
		OutItem = Inventory[Index];
		return true;
	}
	else
	{
		OutItem = FInventorySlot();
		return false;
	}
}

bool UInventoryComponent::CanItemsStack(FInventorySlot& ItemInInventory, FInventorySlot& NewItem, int& Remainder) const
{
	if (ItemInInventory.Item.Id != NewItem.Item.Id)
	{
		Remainder = -1;
		return false;
	}

	int TotalQuantity = ItemInInventory.Quantity + NewItem.Quantity;
	if (TotalQuantity <= ItemInInventory.Item.MaxStackSize)
	{
		Remainder = 0;
		return true;
	}
	else
	{
		Remainder = TotalQuantity - ItemInInventory.Item.MaxStackSize;
		return true;
	}
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	
}

