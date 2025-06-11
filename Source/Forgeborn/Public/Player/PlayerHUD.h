// Copyright Silvergrove Studio

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

class UPlayerHUDWidget;
class UMainWidget;
class UActivatableWidgetBase;
/**
 * Pushes starting widgets to the screen at begin play
 */
UCLASS()
class FORGEBORN_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References")
	TObjectPtr<UMainWidget> MainWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References")
	TObjectPtr<UPlayerHUDWidget> PlayerHUDWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UMainWidget> MainWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UPlayerHUDWidget> PlayerHUDWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UActivatableWidgetBase> SaveIndicatorWidgetClass;

/*
 * Methods
 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UI")
	TSubclassOf<UActivatableWidgetBase> GetActiveWidgetClass();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UI")
	bool isMenuStackEmpty();

protected:
	virtual void BeginPlay() override;

	void InitializeHUD();
	
};
