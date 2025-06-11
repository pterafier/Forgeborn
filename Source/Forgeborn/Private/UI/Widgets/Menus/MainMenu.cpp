// Copyright Silvergrove Studio


#include "UI/Widgets/Menus/MainMenu.h"

#include "Game/Systems/Save/SaveGameSubsystem.h"

void UMainMenu::UpdateGraphicsSettings()
{
	USaveGameSubsystem::Get(this)->SetGraphicsSettingsFromSaveData();
}

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	FTimerHandle TimerHandle;
	//GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::UpdateGraphicsSettings, 0.2f, false);
}
