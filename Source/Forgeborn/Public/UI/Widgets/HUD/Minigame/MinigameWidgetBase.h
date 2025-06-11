// Copyright Silvergrove Studio

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/ActivatableWidgetBase.h"
#include "MinigameWidgetBase.generated.h"

class APlayerControllerBase;

USTRUCT(BlueprintType)
struct FQTEConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinTimeForSuccess = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxTimeForSuccess = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName CommonActionName = NAME_None;
};

UENUM(BlueprintType)
enum class EFailureReason : uint8
{
	None		UMETA(DisplayName = "None"),
	Early		UMETA(DisplayName = "Early"),
	Late		UMETA(DisplayName = "Late"),
	TimeOut		UMETA(DisplayName = "TimeOut"),
	WrongInput	UMETA(DisplayName = "Wrong Input")
};

/**
 * 
 */
UCLASS()
class FORGEBORN_API UMinigameWidgetBase : public UActivatableWidgetBase
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float StoppedTimeValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float QuickTimeEventDuration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MinElapsedTimeForSuccess;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float MaxElapsedTimeForSuccess;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 NumQuickTimeEvents;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<bool> SuccessArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool Success;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool MinigameStarted;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool PressedCorrectInput;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EFailureReason CurrentFailState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FTimerHandle CountdownTimerHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FTimerHandle TimerHandle;

	UFUNCTION(BlueprintImplementableEvent)
	void OnResultsCalculated();

	UFUNCTION(BlueprintCallable)
	void StartQuicktimeEventTimers();

	UFUNCTION(BlueprintCallable)
	void EndQuickTimeEvent();

	UFUNCTION(BlueprintCallable)
	float GetTimerResults();
	
	void StartCountdown();
	void StartTimer(float Duration);
	void StopTimer(FTimerHandle InTimerHandle);
	
	void EventTimeout();

	float StartAngle;
	float EndAngle;

	TObjectPtr<APlayerControllerBase> PlayerControllerBase;

	protected:
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;
	
};
