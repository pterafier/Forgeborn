// Copyright Silvergrove Studio

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/Menus/MenuBase.h"
#include "InventoryGrid.generated.h"

class UCommonButtonBase;
class UCommonButtonGroupBase;
/**
 * 
 */
UCLASS()
class FORGEBORN_API UInventoryGrid : public UMenuBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TObjectPtr<UCommonButtonGroupBase> SlotsButtonGroup;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (BindWidget))
	TObjectPtr<UCommonButtonBase> Slot0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (BindWidget))
	TObjectPtr<UCommonButtonBase> Slot1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (BindWidget))
	TObjectPtr<UCommonButtonBase> Slot2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (BindWidget))
	TObjectPtr<UCommonButtonBase> Slot3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (BindWidget))
	TObjectPtr<UCommonButtonBase> Slot4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (BindWidget))
	TObjectPtr<UCommonButtonBase> Slot5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (BindWidget))
	TObjectPtr<UCommonButtonBase> Slot6;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (BindWidget))
	TObjectPtr<UCommonButtonBase> Slot7;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (BindWidget))
	TObjectPtr<UCommonButtonBase> Slot8;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (BindWidget))
	TObjectPtr<UCommonButtonBase> Slot9;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (BindWidget))
	TObjectPtr<UCommonButtonBase> Slot10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (BindWidget))
	TObjectPtr<UCommonButtonBase> Slot11;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (BindWidget))
	TObjectPtr<UCommonButtonBase> Slot12;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (BindWidget))
	TObjectPtr<UCommonButtonBase> Slot13;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (BindWidget))
	TObjectPtr<UCommonButtonBase> Slot14;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (BindWidget))
	TObjectPtr<UCommonButtonBase> Slot15;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (BindWidget))
	TObjectPtr<UCommonButtonBase> Slot16;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (BindWidget))
	TObjectPtr<UCommonButtonBase> Slot17;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	TArray<TObjectPtr<UCommonButtonBase>> InventoryButtons;

	UFUNCTION()
	void HandleSlotButtonClicked(UCommonButtonBase* Button, int32 ButtonIndex);

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	
};
