// Copyright Silvergrove Studio


#include "UI/Widgets/HUD/Minigame/MinigameWidgetBase.h"

#include "Kismet/GameplayStatics.h"
#include "Player/PlayerControllerBase.h"

void UMinigameWidgetBase::StartQuicktimeEventTimers()
{
	StartCountdown();
	MinigameStarted = true;
}

void UMinigameWidgetBase::EndQuickTimeEvent()
{
	StopTimer(CountdownTimerHandle);
	GetTimerResults();
	
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "Countdown Timer stopped");
}

void UMinigameWidgetBase::StartCountdown()
{
	GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, this, &ThisClass::EventTimeout, QuickTimeEventDuration);
	StartTimer(10.0f);

	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "Countdown started");
}

void UMinigameWidgetBase::StartTimer(float Duration)
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, Duration, false);
	
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "Timer started");
}

void UMinigameWidgetBase::StopTimer(FTimerHandle InTimerHandle)
{
	GetWorld()->GetTimerManager().ClearTimer(InTimerHandle);
}

float UMinigameWidgetBase::GetTimerResults()
{
	if (!MinigameStarted) return -1.f;
	
	StoppedTimeValue = GetWorld()->GetTimerManager().GetTimerElapsed(TimerHandle);
	StopTimer(TimerHandle);

	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "Timer stopped");
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Time elapsed: %f"), StoppedTimeValue));

	if (StoppedTimeValue >= MinElapsedTimeForSuccess && StoppedTimeValue <= MaxElapsedTimeForSuccess && PressedCorrectInput)
	{
		Success = true;
		CurrentFailState = EFailureReason::None;
	}
	else
	{
		if (StoppedTimeValue < MinElapsedTimeForSuccess && PressedCorrectInput)
		{
			CurrentFailState = EFailureReason::Early;
		}
		else if (StoppedTimeValue > MaxElapsedTimeForSuccess && PressedCorrectInput)
		{
			CurrentFailState = EFailureReason::Late;
		}
		else
		{
			CurrentFailState = EFailureReason::WrongInput;
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "Wrong input");
		}
		
		Success = false;
	}

	OnResultsCalculated();
	return StoppedTimeValue;
}

void UMinigameWidgetBase::EventTimeout()
{
	StopTimer(TimerHandle);
	CurrentFailState = EFailureReason::TimeOut;

	Success = false;
	
	OnResultsCalculated();

	//MinigameStarted = false;
	
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "Time ran out!");
}

void UMinigameWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerControllerBase = Cast<APlayerControllerBase>(UGameplayStatics::GetPlayerController(this, 0));
}

void UMinigameWidgetBase::NativePreConstruct()
{
	Super::NativePreConstruct();

	StartAngle = (MinElapsedTimeForSuccess / QuickTimeEventDuration) * 360.0f;
}
