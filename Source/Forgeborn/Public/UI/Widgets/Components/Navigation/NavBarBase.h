// Copyright Silvergrove Studio

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "NavBarBase.generated.h"

/**
 * 
 */
UCLASS()
class FORGEBORN_API UNavBarBase : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Navigation")
	void RemoveParentWidget();
	
};
