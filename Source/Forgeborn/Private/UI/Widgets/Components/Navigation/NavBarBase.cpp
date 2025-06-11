// Copyright Silvergrove Studio


#include "UI/Widgets/Components/Navigation/NavBarBase.h"

#include "Kismet/GameplayStatics.h"
#include "Player/PlayerHUD.h"
#include "UI/Widgets/MainWidget.h"
#include "UI/Widgets/Menus/MenuBase.h"

void UNavBarBase::RemoveParentWidget()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	APlayerHUD* PlayerHUD = Cast<APlayerHUD>(PlayerController->GetHUD());
	UMenuBase* ParentWidget = GetTypedOuter<UMenuBase>();
	UMainWidget* MainWidget = PlayerHUD->MainWidget;

	if (ParentWidget && MainWidget)
	{
		MainWidget->RemoveWidget(ParentWidget);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UNavBarBase: ParentWidget or MainWidget is null, could not activate back action"));
	}
}
