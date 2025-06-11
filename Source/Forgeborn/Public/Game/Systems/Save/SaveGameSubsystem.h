// Copyright Silvergrove Studio

#pragma once

#include "CoreMinimal.h"
#include "SaveGameInterface.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveGameSubsystem.generated.h"

enum class EFullscreenMode : uint8;
struct FAudioSave;
struct FGraphicsSave;
struct FControlsSave;
class USaveGameObject;
class UCommonActivatableWidget;
class UActivatableWidgetBase;
class USaveGame;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSaveGameObjectLoadedDelegate);

/**
 * 
 */
UCLASS()
class FORGEBORN_API USaveGameSubsystem : public UGameInstanceSubsystem, public ISaveGameInterface
{
	GENERATED_BODY()

public:
	USaveGameSubsystem();

/*
 * Variables
 */
	// The object that stores the actual data
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Save")
	TObjectPtr<USaveGameObject> SaveGameObject;

	// The save name of the slot we want to save to
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Save")
	FString SaveSlotName;

	// Useful if we need to change save game objects, otherwise this will always be set to UArsenicSaveGameObject
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Save")
	TSubclassOf<USaveGame> SaveObjectClass;
	
	// Used to dictate the state of various buttons in the UI and objects in the world
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Save")
	bool DoesSaveExist;

	// Used to check that the save object has been properly initialized
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Save")
	bool SaveObjectInitialized;

	// Helper bool to allow checkpoints to know when they can and cannot save based on the state of the save game object
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Save")
	bool CanSave;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Save")
	bool isNewSaveGameObject;

	UPROPERTY(EditDefaultsOnly, Category = "Save")
	TSubclassOf<UActivatableWidgetBase> SaveIndicatorWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Save")
	TObjectPtr<UCommonActivatableWidget> SaveIndicatorWidget;

	UPROPERTY(BlueprintAssignable)
	FOnSaveGameObjectLoadedDelegate OnSaveGameObjectLoaded;

/*
 * Methods
 */
	// Returns the instance of the subsystem
	static USaveGameSubsystem* Get(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, Category = "Save")
	void SetSaveIndicatorWidgetClass(TSubclassOf<UActivatableWidgetBase> WidgetClass);

	// override the subsystem's Initialize function
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION()
	void SetGraphicsSettingsFromSaveData();

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void SetResolution();

	// Begin ISaveGameInterface
	virtual USaveGameObject* GetSaveGameData_Implementation() const override;
	virtual void LoadGameData_Implementation() override;
	virtual void SaveGameData_Implementation() override;
	virtual void SetPlayerSaveData_Implementation(const FPlayerSave& PlayerData) override;
	virtual void SetPlayerAttributes_Implementation(const FCharacterAttributes& PlayerAttributes) override;
	virtual void SetControlsSaveData_Implementation(const FControlsSave& ControlsData) override;
	virtual void SetGraphicsSaveData_Implementation(const FGraphicsSave& GraphicsData) override;
	virtual void SetAudioSettingsSaveData_Implementation(const FAudioSave& AudioData) override;
	// End interface

protected:
	UFUNCTION()
	void OnSaveGameCompleted(const FString& SlotName, const int32 UserIndex, bool bSuccess);
	
	UFUNCTION()
	void OnLoadGameCompleted(const FString& SlotName, const int32 UserIndex, USaveGame* LoadedSaveGameObject);

	void PushSaveIndicatorWidget();
	
};
