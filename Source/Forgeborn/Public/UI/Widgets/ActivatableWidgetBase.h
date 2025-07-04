// Copyright Silvergrove Studio

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ActivatableWidgetBase.generated.h"

struct FUIInputConfig;

UENUM(BlueprintType)
enum class EWidgetInputMode : uint8
{
	Default,
	GameAndMenu,
	Game,
	Menu
};

// An activatable widget that automatically drives the desired input config when activated
UCLASS(Abstract, Blueprintable)
class FORGEBORN_API UActivatableWidgetBase : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UActivatableWidgetBase(const FObjectInitializer& ObjectInitializer);

public:

	//~UCommonActivatableWidget interface
	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;
	//~End of UCommonActivatableWidget interface


protected:
	/** The desired input mode to use while this UI is activated, for example do you want key presses to still reach the game/player controller? */
	UPROPERTY(EditDefaultsOnly, Category = Input)
	EWidgetInputMode InputConfig = EWidgetInputMode::Default;

	/** The desired mouse behavior when the game gets input. */
	UPROPERTY(EditDefaultsOnly, Category = Input)
	EMouseCaptureMode GameMouseCaptureMode = EMouseCaptureMode::CapturePermanently;
	
};
