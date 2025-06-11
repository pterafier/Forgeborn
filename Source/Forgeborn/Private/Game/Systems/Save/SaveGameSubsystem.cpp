// Copyright Silvergrove Studio


#include "Game/Systems/Save/SaveGameSubsystem.h"

#include "Game/Systems/Save/SaveGameObject.h"
#include "Game/Systems/Save/Data/GeneralSettingsSaveData.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerControllerBase.h"
#include "UI/Widgets/MainWidget.h"

USaveGameSubsystem::USaveGameSubsystem()
{
	SaveGameObject = nullptr;
	SaveSlotName = TEXT("SaveSlot");
	SaveObjectClass = USaveGameObject::StaticClass();
	DoesSaveExist = false;
	SaveObjectInitialized = false;
	CanSave = false;
	isNewSaveGameObject = false;
}

USaveGameSubsystem* USaveGameSubsystem::Get(const UObject* WorldContextObject)
{
	if (!WorldContextObject) return nullptr;

	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	if (!GameInstance) return nullptr;

	return GameInstance->GetSubsystem<USaveGameSubsystem>();
}

void USaveGameSubsystem::SetSaveIndicatorWidgetClass(TSubclassOf<UActivatableWidgetBase> WidgetClass)
{
	SaveIndicatorWidgetClass = WidgetClass;
}

void USaveGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	LoadGameData_Implementation();
}

void USaveGameSubsystem::SetGraphicsSettingsFromSaveData()
{
	if (SaveGameObject)
	{
		FGraphicsSave GraphicsSettings = SaveGameObject->GraphicsData;
		UGameUserSettings* GameUserSettings = UGameUserSettings::GetGameUserSettings();
		
		SetResolution();
		GameUserSettings->SetOverallScalabilityLevel(GraphicsSettings.QualityIndex);
		GameUserSettings->SetFrameRateLimit(GraphicsSettings.FrameLimitIndex);
		GameUserSettings->SetViewDistanceQuality(GraphicsSettings.ViewDistanceIndex);
		GameUserSettings->SetAntiAliasingQuality(GraphicsSettings.AntiAliasingIndex);
		GameUserSettings->SetPostProcessingQuality(GraphicsSettings.PostProcessingIndex);
		GameUserSettings->SetShadowQuality(GraphicsSettings.ShadowsIndex);
		GameUserSettings->SetTextureQuality(GraphicsSettings.TexturesIndex);
		GameUserSettings->SetVisualEffectQuality(GraphicsSettings.EffectsIndex);
		GameUserSettings->SetShadingQuality(GraphicsSettings.ShadingIndex);
		GameUserSettings->SetFoliageQuality(GraphicsSettings.FoliageIndex);
		GameUserSettings->SetReflectionQuality(GraphicsSettings.ReflectionsIndex);
		GameUserSettings->SetGlobalIlluminationQuality(GraphicsSettings.GlobalIlluminationIndex);
		GameUserSettings->ApplyResolutionSettings(false);
		GameUserSettings->ApplySettings(false);
	}
}

void USaveGameSubsystem::SetResolution()
{
	FGraphicsSave GraphicsSettings = SaveGameObject->GraphicsData;
	UGameUserSettings* GameUserSettings = UGameUserSettings::GetGameUserSettings();

	if (isNewSaveGameObject)
	{
		GameUserSettings->SetScreenResolution(GameUserSettings->GetDesktopResolution());
		return;
	}
	
	switch (GraphicsSettings.ResolutionIndex)
	{
	case 0:
		GameUserSettings->SetScreenResolution(FIntPoint(1280, 720));
		break;
	case 1:
		GameUserSettings->SetScreenResolution(FIntPoint(1920, 1080));
		break;
	case 2:
		GameUserSettings->SetScreenResolution(FIntPoint(2560, 1440));
		break;
	case 3:
		GameUserSettings->SetScreenResolution(FIntPoint(3840, 2160));
		break;
	default:
		GameUserSettings->SetScreenResolution(FIntPoint(1920, 1080));
	}
}

USaveGameObject* USaveGameSubsystem::GetSaveGameData_Implementation() const
{
	return SaveGameObject;
}

void USaveGameSubsystem::LoadGameData_Implementation()
{
	// Check if the save game exists
	DoesSaveExist = UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0);

	// If it does, load it asynchronously and bind to the OnGameLoaded delegate
	if (DoesSaveExist)
	{
		isNewSaveGameObject = false;
		FAsyncLoadGameFromSlotDelegate OnGameLoadedDelegate;
		OnGameLoadedDelegate.BindUObject(this, &ThisClass::OnLoadGameCompleted);
		UGameplayStatics::AsyncLoadGameFromSlot(SaveSlotName, 0, OnGameLoadedDelegate);
	}
	// Otherwise, create a new save game object
	else
	{
		isNewSaveGameObject = true;
		USaveGame* NewSaveGameObject = UGameplayStatics::CreateSaveGameObject(SaveObjectClass);
		SaveGameObject = Cast<USaveGameObject>(NewSaveGameObject);
		SaveObjectInitialized = true;
		OnSaveGameObjectLoaded.Broadcast();
		CanSave = true;
	}
}

void USaveGameSubsystem::SaveGameData_Implementation()
{
	//PushSaveIndicatorWidget();
	
	FAsyncSaveGameToSlotDelegate OnGameSavedDelegate;
	OnGameSavedDelegate.BindUObject(this, &ThisClass::OnSaveGameCompleted);
	UGameplayStatics::AsyncSaveGameToSlot(SaveGameObject, SaveSlotName, 0, OnGameSavedDelegate);
}

void USaveGameSubsystem::SetPlayerSaveData_Implementation(const FPlayerSave& PlayerData)
{
	SaveGameObject->PlayerData = PlayerData;
}

void USaveGameSubsystem::SetPlayerAttributes_Implementation(const FCharacterAttributes& PlayerAttributes)
{
	SaveGameObject->PlayerAttributes = PlayerAttributes;
}

void USaveGameSubsystem::SetControlsSaveData_Implementation(const FControlsSave& ControlsData)
{
	SaveGameObject->ControlsData = ControlsData;
}

void USaveGameSubsystem::SetGraphicsSaveData_Implementation(const FGraphicsSave& GraphicsData)
{
	SaveGameObject->GraphicsData = GraphicsData;
}

void USaveGameSubsystem::SetAudioSettingsSaveData_Implementation(const FAudioSave& AudioData)
{
	SaveGameObject->AudioData = AudioData;
}

void USaveGameSubsystem::OnSaveGameCompleted(const FString& SlotName, const int32 UserIndex, bool bSuccess)
{
	if (SaveIndicatorWidget)
	{
		APlayerControllerBase* ArsenicPlayerController = Cast<APlayerControllerBase>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		UMainWidget* ArsenicMainWidget = ArsenicPlayerController->MainWidget;
		
		ArsenicMainWidget->RemoveWidget(SaveIndicatorWidget);
		//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Cyan, TEXT("USaveGameSubsystem: SaveIndicatorWidget Popped"));
	}
}

void USaveGameSubsystem::OnLoadGameCompleted(const FString& SlotName, const int32 UserIndex, USaveGame* LoadedSaveGameObject)
{
	SaveGameObject = Cast<USaveGameObject>(LoadedSaveGameObject);

	if (!SaveGameObject)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			10,
			FColor::Red,
			TEXT("USaveGameSubsystem: Loading save game data failed, creating new save game object...")
		);
		isNewSaveGameObject = true;
		USaveGame* NewSaveGameObject = UGameplayStatics::CreateSaveGameObject(SaveObjectClass);
		SaveGameObject = Cast<USaveGameObject>(NewSaveGameObject);
	}
	
	SaveObjectInitialized = true;
	OnSaveGameObjectLoaded.Broadcast();
	CanSave = true;
}

void USaveGameSubsystem::PushSaveIndicatorWidget()
{
	check(SaveIndicatorWidgetClass);
	
	APlayerControllerBase* PlayerController = Cast<APlayerControllerBase>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	UMainWidget* MainWidget = PlayerController->MainWidget;
	UCommonActivatableWidgetContainerBase* PopupStack = MainWidget->Popup_Stack;

	SaveIndicatorWidget = MainWidget->PushWidgetToTargetStack(PopupStack, SaveIndicatorWidgetClass, false, false);
	//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Cyan, TEXT("USaveGameSubsystem: SaveIndicatorWidget Pushed"));
}
