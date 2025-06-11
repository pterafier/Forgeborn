// Copyright Silvergrove Studio

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SaveGameInterface.generated.h"

class USaveGameObject;
struct FCharacterAttributes;
struct FGraphicsSave;
struct FControlsSave;
struct FAudioSave;
struct FPlayerSave;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USaveGameInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FORGEBORN_API ISaveGameInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// Function used to return save data from the class that implements this interface
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Save")
	USaveGameObject* GetSaveGameData() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Save")
	void LoadGameData();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Save")
	void SaveGameData();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Save")
	void SetPlayerSaveData(const FPlayerSave& PlayerData);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Save")
	void SetPlayerAttributes(const FCharacterAttributes& PlayerAttributes);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Save")
	void SetControlsSaveData(const FControlsSave& SavedControlsData);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Save")
	void SetGraphicsSaveData(const FGraphicsSave& GraphicsData);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Save")
	void SetAudioSettingsSaveData(const FAudioSave& AudioData);

	virtual USaveGameObject* GetSaveGameData_Implementation() const = 0;
	virtual void LoadGameData_Implementation() = 0;
	virtual void SaveGameData_Implementation() = 0;
	virtual void SetPlayerSaveData_Implementation(const FPlayerSave& PlayerData) = 0;
	virtual void SetPlayerAttributes_Implementation(const FCharacterAttributes& PlayerAttributes) = 0;
	virtual void SetControlsSaveData_Implementation(const FControlsSave& ControlsData) = 0;
	virtual void SetGraphicsSaveData_Implementation(const FGraphicsSave& GraphicsData) = 0;
	virtual void SetAudioSettingsSaveData_Implementation(const FAudioSave& AudioData) = 0;
};
