// Copyright Silvergrove Studio

#pragma once

#include "CoreMinimal.h"
#include "SaveGameInterface.h"
#include "Characters/CharacterBase.h"
#include "Data/GeneralSettingsSaveData.h"
#include "GameFramework/SaveGame.h"
#include "Player/PlayerControllerBase.h"
#include "Player/PlayerStateBase.h"
#include "SaveGameObject.generated.h"

/**
 * 
 */
UCLASS()
class FORGEBORN_API USaveGameObject : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Save")
	FAudioSave AudioData;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Save")
	FGraphicsSave GraphicsData;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Save")
	FPlayerSave PlayerData;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Save")
	FCharacterAttributes PlayerAttributes;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Save")
	FControlsSave ControlsData;
	
};
