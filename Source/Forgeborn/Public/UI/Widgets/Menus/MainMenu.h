// Copyright Silvergrove Studio

#pragma once

#include "CoreMinimal.h"
#include "MenuBase.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class FORGEBORN_API UMainMenu : public UMenuBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void UpdateGraphicsSettings();

protected:
	virtual void NativeConstruct() override;
	
};
