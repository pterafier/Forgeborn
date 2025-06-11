// Copyright Silvergrove Studio


#include "Player/PlayerControllerBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "VectorTypes.h"
#include "Actors/Interaction/WorkstationBase.h"
#include "Camera/CameraComponent.h"
#include "Characters/PlayerCharacter.h"
#include "Game/Systems/Save/SaveGameObject.h"
#include "Game/Systems/Save/SaveGameSubsystem.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "GAS/AbilitySystemComponentBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerHUD.h"
#include "UI/Widgets/MainWidget.h"
#include "Framework/Application/SlateApplication.h"

APlayerControllerBase::APlayerControllerBase()
{
	BaseTurnRate = 1.0f;
	InvertY = false;
	isMenuActivated = false;
	BoundToGameLoadedDelegate = false;
	TargetArmLengthMax = 2500.f;
	TargetArmLengthMin = 500.f;
	ZoomStepAmount = 150.f;
	ZoomInterpolationRate = 20.f;
	TargetZoomLength = 1.0f;
	InMinigame = false;
	LastRotation = FRotator::ZeroRotator;
	isBlendingViewTarget = false;
}

void APlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();

	// get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(GameInputMappingContext, 0);
		CurrentInputMappingContext = GameInputMappingContext;
		//GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Green, TEXT("Mapping context added successfully"));
	}

	SetReferences();
	SetupInputComponent(InputComponent);
	bEnableClickEvents = true;
}

void APlayerControllerBase::SetupInputComponent(UInputComponent* InInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InInputComponent))
	{
		// Jumping
		//EnhancedInputComponent->BindAction(SpacebarAction, ETriggerEvent::Started, UGameplayStatics::GetPlayerCharacter(this, 0), &ACharacter::Jump);
		//EnhancedInputComponent->BindAction(SpacebarAction, ETriggerEvent::Completed, UGameplayStatics::GetPlayerCharacter(this, 0), &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);

		// Looking
		//EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);

		// Opening and closing pause menu
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &ThisClass::HandlePauseMenu);

		EnhancedInputComponent->BindAction(TabLeftAction, ETriggerEvent::Started, this, &ThisClass::OnTabLeftPressed);
		EnhancedInputComponent->BindAction(TabRightAction, ETriggerEvent::Started, this, &ThisClass::OnTabRightPressed);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ThisClass::CallPlayerInteract);
		EnhancedInputComponent->BindAction(ZoomCameraInAction, ETriggerEvent::Started, this, &ThisClass::StartZoomCameraIn);
		EnhancedInputComponent->BindAction(ZoomCameraOutAction, ETriggerEvent::Started, this, &ThisClass::StartZoomCameraOut);
		EnhancedInputComponent->BindAction(ExitWorkstationAction, ETriggerEvent::Started, this, &ThisClass::CallExitWorkstation);
		EnhancedInputComponent->BindAction(RightClickAction, ETriggerEvent::Started, this, &ThisClass::OnRightClickDown);
		EnhancedInputComponent->BindAction(LeftClickAction, ETriggerEvent::Started, this, &ThisClass::OnLeftClickDown);
		EnhancedInputComponent->BindAction(SpacebarAction, ETriggerEvent::Started, this, &ThisClass::OnSpacebarDown);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Red, TEXT("SetupInputComponent() failed!"));
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void APlayerControllerBase::HUDInitialized()
{
	PlayerHUDActor = Cast<APlayerHUD>(GetHUD());
	MainWidget = PlayerHUDActor->MainWidget;

	check(MainWidget);
	MainWidget->OnStacksCleared.AddDynamic(this, &ThisClass::OnStacksCleared);
}

void APlayerControllerBase::SwitchToGameplayInput()
{
	if (InMinigame) return;
	
	if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (CurrentInputMappingContext == GameInputMappingContext) return;
		
		InputSubsystem->RemoveMappingContext(CurrentInputMappingContext);
		InputSubsystem->AddMappingContext(GameInputMappingContext, 0);
		CurrentInputMappingContext = GameInputMappingContext;
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, FString::Printf(TEXT("%s"), *CurrentInputMappingContext->GetName()));
	}
}

void APlayerControllerBase::SwitchToMenuInput()
{
	if (InMinigame) return;
	
	if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (CurrentInputMappingContext == MenuInputMappingContext) return;
		
		InputSubsystem->RemoveMappingContext(CurrentInputMappingContext);
		InputSubsystem->AddMappingContext(MenuInputMappingContext, 0);
		CurrentInputMappingContext = MenuInputMappingContext;
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, FString::Printf(TEXT("%s"), *CurrentInputMappingContext->GetName()));
	}
}

void APlayerControllerBase::SwitchToMinigameInput()
{
	if (InMinigame) return;
	
	if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (CurrentInputMappingContext == MinigameInputMappingContext) return;
		
		InputSubsystem->RemoveMappingContext(CurrentInputMappingContext); 
		InputSubsystem->AddMappingContext(MinigameInputMappingContext, 0);
		CurrentInputMappingContext = MinigameInputMappingContext;
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, FString::Printf(TEXT("%s"), *CurrentInputMappingContext->GetName()));
	}
}

void APlayerControllerBase::OnTabLeftPressed()
{
	OnLeftTabPressed.Broadcast();
}

void APlayerControllerBase::OnTabRightPressed()
{
	OnRightTabPressed.Broadcast();
}

FHitResult APlayerControllerBase::GetActorUnderMouseCursor()
{
	FHitResult CursorHit;
	
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return CursorHit;

	//DrawDebugSphere(GetWorld(), CursorHit.ImpactPoint, 50.f, 12, FColor::Red);

	return CursorHit;
}

void APlayerControllerBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void APlayerControllerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotateCharacterToMouseCursor();
}

void APlayerControllerBase::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	FRotator CameraRotation = GetControlRotation();

	// Only keep the yaw to ensure movement is horizontal
	FRotator YawRotation(0.f, CameraRotation.Yaw, 0.f);

	// Get camera-forward/right vectors projected onto the horizontal plane
	FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// Apply movement input relative to camera orientation
	PlayerPawn->AddMovementInput(ForwardDirection, MovementVector.Y);
	PlayerPawn->AddMovementInput(RightDirection, MovementVector.X);
}

void APlayerControllerBase::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// add yaw and pitch input to controller
	PlayerPawn->AddControllerYawInput(LookAxisVector.X * BaseTurnRate);
	
	float InvertedPitchInput = LookAxisVector.Y * BaseTurnRate * -1;
	float PitchInput = LookAxisVector.Y * BaseTurnRate;

	// if InvertY = true, set Result to InvertedPitchInput
	float Result = InvertY ? InvertedPitchInput : PitchInput; 
	PlayerPawn->AddControllerPitchInput(Result);
}

void APlayerControllerBase::OnRightClickDown()
{
	OnRightClickPressed.Broadcast();
}

void APlayerControllerBase::OnLeftClickDown()
{
	OnLeftClickPressed.Broadcast();
}

void APlayerControllerBase::OnSpacebarDown()
{
	OnSpacebarPressed.Broadcast();
}

void APlayerControllerBase::OnRightClickReleased()
{
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
	SetShowMouseCursor(true);
}

void APlayerControllerBase::StartZoomCameraIn()
{
	float NewTarget = PlayerCharacter->SpringArm->TargetArmLength - ZoomStepAmount;
	TargetZoomLength = FMath::Clamp(NewTarget, TargetArmLengthMin, TargetArmLengthMax);
	GetWorldTimerManager().SetTimer(CameraZoomTimerHandle, this, &ThisClass::InterpZoomCameraIn, GetWorld()->GetDeltaSeconds(), true);
}

void APlayerControllerBase::StartZoomCameraOut()
{
	float NewTarget = PlayerCharacter->SpringArm->TargetArmLength + ZoomStepAmount;
	TargetZoomLength = FMath::Clamp(NewTarget, TargetArmLengthMin, TargetArmLengthMax);
	GetWorldTimerManager().SetTimer(CameraZoomTimerHandle, this, &ThisClass::InterpZoomCameraIn, GetWorld()->GetDeltaSeconds(), true);
}

void APlayerControllerBase::InterpZoomCameraIn()
{
	float& Current = PlayerCharacter->SpringArm->TargetArmLength;
	Current = FMath::FInterpTo(Current, TargetZoomLength, GetWorld()->GetDeltaSeconds(), ZoomInterpolationRate);

	if (FMath::IsNearlyEqual(Current, TargetZoomLength, 1.f))
	{
		Current = TargetZoomLength;
		GetWorldTimerManager().ClearTimer(CameraZoomTimerHandle);
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Cyan, TEXT("PlayerControllerBase: Camera timer cleared"));
	}
}

void APlayerControllerBase::InterpZoomCameraOut()
{
	float TargetLength = PlayerCharacter->SpringArm->TargetArmLength + ZoomStepAmount;
	float ClampedTarget = FMath::Clamp(TargetLength, TargetArmLengthMin, TargetArmLengthMax);
	PlayerCharacter->SpringArm->TargetArmLength = FMath::FInterpTo(PlayerCharacter->SpringArm->TargetArmLength, ClampedTarget, GetWorld()->GetDeltaSeconds(), ZoomInterpolationRate);

	if (PlayerCharacter->SpringArm->TargetArmLength == PlayerCharacter->SpringArm->TargetArmLength + ZoomStepAmount - 10)
	{
		GetWorldTimerManager().ClearTimer(CameraZoomTimerHandle);

		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Cyan, TEXT("PlayerControllerBase: Camera timer cleared"));
	}
}

void APlayerControllerBase::HandlePauseMenu()
{
	if (isBlendingViewTarget) return;
	
	TSubclassOf<UActivatableWidgetBase> ActiveWidget = PlayerHUDActor->GetActiveWidgetClass();

	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Cyan, TEXT("PlayerControllerBase: Pause Menu button triggered"));
	
	if (ActiveWidget == PauseMenu || PlayerHUDActor->isMenuStackEmpty())
	{
		isMenuActivated = !isMenuActivated;
		if (isMenuActivated)
		{
			SwitchToMenuInput();
			UCommonActivatableWidgetContainerBase* TargetStack = MainWidget->Menu_Stack;
			PauseMenuInstance = MainWidget->PushWidgetToTargetStack(TargetStack, PauseMenu, true, true);
		}
		else
		{
			if (ActiveWidget == PauseMenu)
			{
				MainWidget->RemoveWidget(PauseMenuInstance.Get());
				PauseMenuInstance = nullptr;
			}
		}
	}
}

void APlayerControllerBase::OnStacksCleared()
{
	if (InMinigame)
	{
		SwitchToMinigameInput();
		return;
	}
	
	isMenuActivated = false;
	SwitchToGameplayInput();

	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT("Stacks Cleared!"));
}

void APlayerControllerBase::CallPlayerInteract()
{
	PlayerCharacter->Interact();
}

void APlayerControllerBase::CallExitWorkstation()
{
	if (CurrentWorkstation && !isBlendingViewTarget)
	{
		CurrentWorkstation->PlayerExitWorkstation();
	}
}

void APlayerControllerBase::SetCurrentWorkstation(AWorkstationBase* Workstation)
{
	CurrentWorkstation = Workstation;
}

void APlayerControllerBase::SetPlayerInMinigame(bool isInMinigame)
{
	if (isInMinigame)
	{
		SwitchToMinigameInput();
		InMinigame = true;
		EnableInput(this);
	}
	else
	{
		InMinigame = false;
		SwitchToGameplayInput();
	}
}

// Called from the player state when save data is initialized so we can set the look speed at the right time
void APlayerControllerBase::SetControlsFromSaveData()
{
	USaveGameSubsystem* SaveSubsystem = USaveGameSubsystem::Get(this);
	USaveGameObject* SaveGameObject = ISaveGameInterface::Execute_GetSaveGameData(SaveSubsystem);

	FControlsSave PlayerControls = SaveGameObject->ControlsData;
	InvertY = PlayerControls.InvertY;

	UAbilitySystemComponentBase* AbilitySystem = Cast<UAbilitySystemComponentBase>(PlayerCharacter->GetAbilitySystemComponent());
	if (AbilitySystem)
	{
		TMap<FGameplayTag, float> SetByCallerMagnitudes;
		FGameplayTag LookSpeedTag = FGameplayTag::RequestGameplayTag(FName("Attributes.LookSpeed"));
		FGameplayTag BaseLookSpeedTag = FGameplayTag::RequestGameplayTag(FName("Attributes.BaseLookSpeed"));
		SetByCallerMagnitudes.Add(LookSpeedTag, PlayerControls.BaseTurnRate);
		SetByCallerMagnitudes.Add(BaseLookSpeedTag, PlayerControls.BaseTurnRate);

		check (InitializeLookSpeedEffect);
		AbilitySystem->ApplyEffectToTarget(PlayerCharacter, InitializeLookSpeedEffect, 1.0f, SetByCallerMagnitudes);

		//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Cyan, FString::Printf(TEXT("PlayerControllerBase: LookSpeed: %f"), PlayerControls.BaseTurnRate));
	}
}

void APlayerControllerBase::SetReferences()
{
	PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	
	check(PauseMenu); // this gets set in blueprint so we need to make sure it's set, this is a great place to assert that
}

void APlayerControllerBase::RotateCharacterToMouseCursor()
{
	if (!InMinigame && CurrentInputMappingContext->GetName() != "IMC_UI")
	{
		FVector Velocity = PlayerCharacter->GetVelocity();
		Velocity.Z = 0;

		if (!Velocity.IsNearlyZero())
		{
			FRotator CurrentRotation = PlayerCharacter->GetMesh()->GetComponentRotation();
			FRotator TargetRotation = Velocity.GetSafeNormal().Rotation();
			TargetRotation.Yaw -= 90.0f;

			// Only yaw is needed for horizontal rotation
			FRotator SmoothedRotation = FMath::RInterpTo(
				CurrentRotation,
				FRotator(0.f, TargetRotation.Yaw, 0.f),
				GetWorld()->GetDeltaSeconds(),
				15.0f
			);

			PlayerCharacter->GetMesh()->SetWorldRotation(SmoothedRotation);
		}
	}
#if 0
	if (!InMinigame && CurrentInputMappingContext->GetName() != "IMC_UI")
	{
		FHitResult HitResult = GetActorUnderMouseCursor();
		if (HitResult.bBlockingHit)
		{
			FVector PlayerLocation = PlayerCharacter->GetActorLocation();
			FVector HitActorLocation = HitResult.ImpactPoint;
			FVector TargetDirection = (HitActorLocation - PlayerLocation).GetSafeNormal();
			FRotator Rotation = TargetDirection.Rotation();
			LastRotation = Rotation;
		}
		PlayerCharacter->GetMesh()->SetWorldRotation(FRotator(0, LastRotation.Yaw, 0));
	}
#endif
}

