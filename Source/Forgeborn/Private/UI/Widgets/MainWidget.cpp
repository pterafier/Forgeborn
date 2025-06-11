// Copyright Silvergrove Studio


#include "UI/Widgets/MainWidget.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Player/PlayerControllerBase.h"

UCommonActivatableWidget* UMainWidget::PushWidgetToTargetStack(
    UCommonActivatableWidgetContainerBase* TargetStack, TSubclassOf<UCommonActivatableWidget> WidgetClass,
    bool bActivateWidget, bool DisableInput, bool FlushInput)
{
    if (bActivateWidget)
    {
        ActivateWidget();
        PushedWidget = TargetStack->AddWidget(WidgetClass);
        PushedWidget->ActivateWidget();
        SetInputModeMenuOpened(PushedWidget, DisableInput, FlushInput);
    }
    else
    {
        PushedWidget = TargetStack->AddWidget(WidgetClass);
        PushedWidget->ActivateWidget();
    }
    return PushedWidget;
}

void UMainWidget::SetInputModeMenuOpened(UWidget* WidgetToFocus, bool DisableInput, bool FlushInput)
{
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
    FInputModeGameAndUI InputModeGameAndUI;
    
    if (FlushInput)
    {
        PlayerController->FlushPressedKeys();
    }
    
    InputModeGameAndUI.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    InputModeGameAndUI.SetHideCursorDuringCapture(false);
    InputModeGameAndUI.SetWidgetToFocus(WidgetToFocus->TakeWidget());
    PlayerController->SetInputMode(InputModeGameAndUI);
    PlayerController->SetShowMouseCursor(true);
    
    if (DisableInput)
    {
        PlayerCharacter->DisableInput(PlayerController);
    }
}

void UMainWidget::SetInputModeMenuClosed()
{
#if 1
    if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
    {
        FInputModeGameOnly InputMode;
        //InputMode.SetConsumeCaptureMouseDown(false);
        PlayerController->SetInputMode(InputMode);
        PlayerController->SetShowMouseCursor(false);
        
        GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT("Game Only input mode set!"));
        if (ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0))
        {
            PlayerCharacter->EnableInput(PlayerController);
        }
    }
#endif
#if 0
    if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
    {
        FInputModeGameAndUI InputMode;
        InputMode.SetWidgetToFocus(nullptr); // Don't set a widget
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        InputMode.SetHideCursorDuringCapture(false); // Cursor remains visible
        PlayerController->SetInputMode(InputMode);
        //PlayerController->SetShowMouseCursor(true);
        if (ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0))
        {
            PlayerCharacter->EnableInput(PlayerController);
        }
    }
#endif
}

void UMainWidget::RemoveWidget(UCommonActivatableWidget* Widget)
{
    Widget->DeactivateWidget();
    float MenuTransitionDuration = Menu_Stack->GetTransitionDuration();

    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(
        TimerHandle,
        [this]()
        {
            PushedWidget = Menu_Stack->GetActiveWidget();
        },
        MenuTransitionDuration,
        false
    );
}

void UMainWidget::ClearStacks()
{
    if (PlayerControllerBase->InMinigame) return;
    
    Menu_Stack->ClearWidgets();
    //Popup_Stack->ClearWidgets();
    SetInputModeMenuClosed();
    PushedWidget = nullptr;
    OnStacksCleared.Broadcast();
}

void UMainWidget::NativeConstruct()
{
    Super::NativeConstruct();

    PushedWidget == nullptr;
    bAutomaticallyRegisterInputOnConstruction = true;
    PlayerControllerBase = Cast<APlayerControllerBase>(UGameplayStatics::GetPlayerController(this, 0));
}

void UMainWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    
    if (PushedWidget == nullptr && StacksCleared == true)
    {
        ClearStacks();
        StacksCleared = false;
    }
    else if (PushedWidget != nullptr)
    {
        StacksCleared = true;
    }
}

bool UMainWidget::NativeOnHandleBackAction()
{
    return true;
}
