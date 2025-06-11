// Copyright Silvergrove Studio

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/ActivatableWidgetBase.h"
#include "InteractPopup.generated.h"

/**
 * 
 */
UCLASS()
class FORGEBORN_API UInteractPopup : public UActivatableWidgetBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> AnimateIn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> AnimateOut;
};

