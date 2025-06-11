// Copyright Silvergrove Studio


#include "Player/PlayerHUD.h"
#include "UI/Widgets/MainWidget.h"
#include "Blueprint/UserWidget.h"
#include "Game/Systems/Save/SaveGameSubsystem.h"
#include "Player/PlayerControllerBase.h"
#include "UI/Widgets/HUD/PlayerHUDWidget.h"
#include "Widgets/CommonActivatableWidgetContainer.h"

TSubclassOf<UActivatableWidgetBase> APlayerHUD::GetActiveWidgetClass()
{
	if (MainWidget->PushedWidget)
	{
		return MainWidget->PushedWidget->GetClass();
	}
	else
	{
		return nullptr;
	}
}

bool APlayerHUD::isMenuStackEmpty()
{
	return MainWidget->Menu_Stack->GetNumWidgets() == 0;
}

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();

	InitializeHUD();

	check(SaveIndicatorWidgetClass)
	USaveGameSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<USaveGameSubsystem>();
	SaveSubsystem->SetSaveIndicatorWidgetClass(SaveIndicatorWidgetClass);
}

void APlayerHUD::InitializeHUD()
{
	check(MainWidgetClass);
	check(PlayerHUDWidgetClass);
	
	MainWidget = CreateWidget<UMainWidget>(GetWorld()->GetFirstPlayerController(), MainWidgetClass);
	MainWidget->AddToViewport();

	UCommonActivatableWidgetContainerBase* TargetStack = MainWidget->HUD_Stack;
	TSubclassOf<UCommonActivatableWidget> WidgetClass = PlayerHUDWidgetClass;
	UCommonActivatableWidget* NewPlayerHUDWidget = MainWidget->PushWidgetToTargetStack(TargetStack, WidgetClass, false, false);
	PlayerHUDWidget = Cast<UPlayerHUDWidget>(NewPlayerHUDWidget); // Casting so we have an easy reference to the always-active ArsenicPlayerHUD

	MainWidget->SetInputModeMenuClosed();

	Cast<APlayerControllerBase>(GetOwningPlayerController())->HUDInitialized();
}
