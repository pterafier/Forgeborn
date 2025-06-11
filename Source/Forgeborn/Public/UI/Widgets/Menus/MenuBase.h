// Copyright Silvergrove Studio

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/ActivatableWidgetBase.h"
#include "MenuBase.generated.h"

class APlayerControllerBase;
class UMainWidget;

/**
 * 
 */
UCLASS()
class FORGEBORN_API UMenuBase : public UActivatableWidgetBase
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References")
	TObjectPtr<APlayerControllerBase> PlayerControllerBase;
	

protected:
	virtual bool NativeOnHandleBackAction() override;
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;

	UFUNCTION(BlueprintImplementableEvent)
	void PlayerControllerSet();

	UFUNCTION(BlueprintCallable, Category = "State Management")
	void CloseThisMenu();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UMainWidget> MainWidget;

private:
	
	
	
};
