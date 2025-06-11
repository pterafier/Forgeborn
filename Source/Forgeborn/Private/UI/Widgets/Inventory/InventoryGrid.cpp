// Copyright Silvergrove Studio


#include "UI/Widgets/Inventory/InventoryGrid.h"
#include "Groups/CommonButtonGroupBase.h"

void UInventoryGrid::HandleSlotButtonClicked(UCommonButtonBase* Button, int32 ButtonIndex)
{
}

void UInventoryGrid::NativePreConstruct()
{
	Super::NativePreConstruct();

	SlotsButtonGroup = NewObject<UCommonButtonGroupBase>(this);

	InventoryButtons.Add(Slot0);
	InventoryButtons.Add(Slot1);
	InventoryButtons.Add(Slot2);
	InventoryButtons.Add(Slot3);
	InventoryButtons.Add(Slot4);
	InventoryButtons.Add(Slot5);
	InventoryButtons.Add(Slot6);
	InventoryButtons.Add(Slot7);
	InventoryButtons.Add(Slot8);
	InventoryButtons.Add(Slot9);
	InventoryButtons.Add(Slot10);
	InventoryButtons.Add(Slot11);
	InventoryButtons.Add(Slot12);
	InventoryButtons.Add(Slot13);
	InventoryButtons.Add(Slot14);
	InventoryButtons.Add(Slot15);
	InventoryButtons.Add(Slot16);
	InventoryButtons.Add(Slot17);

	SlotsButtonGroup->AddWidgets(InventoryButtons);
	SlotsButtonGroup->SetSelectionRequired(false);
}

void UInventoryGrid::NativeConstruct()
{
	Super::NativeConstruct();
	
	//SlotsButtonGroup->OnButtonBaseClicked().AddDynamic(this, &ThisClass::HandleSlotButtonClicked);
}

void UInventoryGrid::NativeDestruct()
{
	Super::NativeDestruct();

	SlotsButtonGroup->RemoveAll();
	InventoryButtons.Empty();
}

