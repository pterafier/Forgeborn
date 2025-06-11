// Copyright Silvergrove Studio

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/ActivatableWidgetBase.h"
#include "UI/Widgets/Menus/MenuBase.h"
#include "SettingsMenu.generated.h"

class UCommonButtonBase;
class UCommonButtonGroupBase;

/**
 * 
 */
UCLASS()
class FORGEBORN_API USettingsMenu : public UMenuBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Navigation")
	TObjectPtr<UCommonButtonGroupBase> NavButtonGroup;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCommonButtonBase> BTN_Video;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCommonButtonBase> BTN_Audio;

	UPROPERTY(BlueprintReadOnly, Category = "Navigation")
	TArray<TObjectPtr<UCommonButtonBase>> Buttons;

protected:
	virtual void NativePreConstruct() override;
	
};
