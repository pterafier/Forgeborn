// Copyright Silvergrove Studio


#include "UI/Widgets/Menus/MenuBase.h"

#include "Kismet/GameplayStatics.h"
#include "Player/PlayerControllerBase.h"
#include "UI/Widgets/MainWidget.h"

bool UMenuBase::NativeOnHandleBackAction()
{
	if (!MainWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("MenuBase: MainWidget is not valid, could not handle back action"));
		return true; // returning true so inputs don't get hung up - we want to move on if this is the case - not actually tested though so this might break something IDK :O
	}
	else
	{
		MainWidget->RemoveWidget(this);
		return true;
	}
}

void UMenuBase::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerControllerBase = Cast<APlayerControllerBase>(GetWorld()->GetFirstPlayerController());
	if (PlayerControllerBase)
	{
		MainWidget = PlayerControllerBase->MainWidget;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("MenuBase: Cast to PlayerControllerBase failed!"));
	}

	PlayerControllerSet(); // Call the blueprint implementable event so UI can safely bind to PlayerController delegates
}

void UMenuBase::NativePreConstruct()
{
	Super::NativePreConstruct();

	bAutomaticallyRegisterInputOnConstruction = true; 
}

void UMenuBase::CloseThisMenu()
{
	MainWidget->RemoveWidget(this);
}
