// Copyright Silvergrove Studio

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerBase.generated.h"

class AWorkstationBase;
class UGameplayEffect;
class APlayerCharacter;
class APlayerStateBase;
class UInputMappingContext;
struct FInputActionValue;
class UInputAction;
class APlayerHUD;
class UCommonActivatableWidget;
class UActivatableWidgetBase;
class UMainWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLeftTabPressed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRightTabPressed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSpacebarPressed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLeftClickPressed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRightClickPressed);

/**
 * 
 */

// Player Controls
USTRUCT(BlueprintType)
struct FControlsSave : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controls")
	float BaseTurnRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controls")
	bool InvertY;

	FControlsSave() :
		BaseTurnRate(0.5f),
		InvertY(false)
	{}
};

UCLASS()
class FORGEBORN_API APlayerControllerBase : public APlayerController
{
	GENERATED_BODY()
	
public:
/*
 * Public Variables
 */
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* SpacebarAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* PauseAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* TabLeftAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* TabRightAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* InteractAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* RightClickAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* LeftClickAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* ZoomCameraInAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* ZoomCameraOutAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* ExitWorkstationAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	float BaseTurnRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	float TargetArmLengthMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	float TargetArmLengthMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	float ZoomStepAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	float ZoomInterpolationRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Input)
	bool InvertY;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "References")
	APawn* PlayerPawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References")
	TObjectPtr<APlayerStateBase> PlayerStateBase;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References")
	TObjectPtr<APlayerCharacter> PlayerCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References")
	TObjectPtr<UMainWidget> MainWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UActivatableWidgetBase> PauseMenu;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UCommonActivatableWidget> PauseMenuInstance;

	// This gets the ACTOR HUD which is set in the game mode, not the HUD that has progress bars and other information.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<APlayerHUD> PlayerHUDActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI");
	bool isMenuActivated;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI");
	bool InMinigame;

	UPROPERTY(BlueprintAssignable, Category = Input)
	FOnLeftTabPressed OnLeftTabPressed;

	UPROPERTY(BlueprintAssignable, Category = Input)
	FOnRightTabPressed OnRightTabPressed;
	
	UPROPERTY(BlueprintAssignable, Category = Input)
	FOnSpacebarPressed OnSpacebarPressed;
	
	UPROPERTY(BlueprintAssignable, Category = Input)
	FOnLeftClickPressed OnLeftClickPressed;

	UPROPERTY(BlueprintAssignable, Category = Input)
	FOnRightClickPressed OnRightClickPressed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Workstation")
	TObjectPtr<AWorkstationBase> CurrentWorkstation;
	
	bool isBlendingViewTarget;

/*
 * Public Methods
 */
	
	UFUNCTION(BlueprintCallable, Category = "Save")
	void SetControlsFromSaveData();
	
	UFUNCTION(BlueprintCallable)
	void CallExitWorkstation();

	UFUNCTION()
	void SetCurrentWorkstation(AWorkstationBase* Workstation);

	UFUNCTION(BlueprintCallable)
	void SetPlayerInMinigame(bool isInMinigame);
	
	//Constructor
	APlayerControllerBase();

	// Called from the PlayerHUDActor when it's initialized to inform the player controller that it's safe to do widget stuff
	void HUDInitialized();

	// Called when menus are closed to restore gameplay input mapping context
	void SwitchToGameplayInput();

	// Called when menus are opened to use menu input mapping context
	void SwitchToMenuInput();

	// Called when a player is in a minigame
	void SwitchToMinigameInput();

	// These methods are called when the player tries to tab a menu left or right. They must have the correct IMC assigned
	void OnTabLeftPressed();
	void OnTabRightPressed();
	void OnRightClickDown();
	void OnLeftClickDown();
	void OnSpacebarDown();

	FHitResult GetActorUnderMouseCursor();

protected:
/*
 * Protected Methods
 */
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override; // Overridden to create a reference to the player state at the earliest possible time
	virtual void Tick(float DeltaTime) override;
	
	// APawn interface
	void SetupInputComponent(UInputComponent* InputComponent);
	// End of APawn interface

	/** Called for movement input */
	UFUNCTION(BlueprintCallable, Category = Input)
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	UFUNCTION(BlueprintCallable, Category = Input)
	void Look(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = Input)
	void OnRightClickReleased();

	UFUNCTION(BlueprintCallable, Category = Input)
	void StartZoomCameraIn();

	UFUNCTION(BlueprintCallable, Category = Input)
	void StartZoomCameraOut();

	UFUNCTION(BlueprintCallable, Category = Input)
	void InterpZoomCameraIn();

	UFUNCTION(BlueprintCallable, Category = Input)
	void InterpZoomCameraOut();

	/** Called for opening and closing the pause menu */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void HandlePauseMenu();

	// Responsible for setting menu bools back to false. See HandlePauseMenu for example
	UFUNCTION(Category = "UI")
	void OnStacksCleared();

	UFUNCTION()
	void CallPlayerInteract();

/*
 * Protected Variables
 */
	
	/** Input Mapping Context to be used for player input during gameplay not involving menus */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* GameInputMappingContext;

	/** Input Mapping Context to be used for player input during gameplay only involving menus */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* MenuInputMappingContext;

	/** Input Mapping Context to be used for player input during gameplay only involving menus */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* MinigameInputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> InitializeLookSpeedEffect;

	virtual void SetReferences();

	void RotateCharacterToMouseCursor();

	bool BoundToGameLoadedDelegate;

	FRotator LastRotation;

	FTimerHandle CameraZoomTimerHandle;
	float TargetZoomLength;
	
	UInputMappingContext* CurrentInputMappingContext;
};


