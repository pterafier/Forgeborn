// Copyright Silvergrove Studio

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GeneralSettingsSaveData.generated.h"

UENUM(BlueprintType)
enum class EFullscreenMode : uint8
{
	Fullscreen = 0,
	Windowed = 1
};

// Audio Settings
USTRUCT(BlueprintType)
struct FAudioSave : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio Settings")
	float MasterVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio Settings")
	float MusicVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio Settings")
	float SFXVolume;
	
	FAudioSave():
		MasterVolume(1.f),
		MusicVolume(1.f),
		SFXVolume(1.f)
	{}
};

// Graphics Settings
USTRUCT(BlueprintType)
struct FGraphicsSave : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	int ResolutionIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	int FrameLimitIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	int QualityIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	int ViewDistanceIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	int PostProcessingIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	int ShadowsIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	int AntiAliasingIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	int TexturesIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	int EffectsIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	int ShadingIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	int FoliageIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	int ReflectionsIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	int GlobalIlluminationIndex;

	FGraphicsSave() :
		ResolutionIndex(2),
		FrameLimitIndex(2),
		QualityIndex(3),
		ViewDistanceIndex(3),
		PostProcessingIndex(3),
		ShadowsIndex(3),
		AntiAliasingIndex(3),
		TexturesIndex(3),
		EffectsIndex(3),
		ShadingIndex(3),
		FoliageIndex(3),
		ReflectionsIndex(3),
		GlobalIlluminationIndex(3)
	{}
};

/**
 * 
 */
UCLASS()
class FORGEBORN_API UGeneralSettingsSaveData : public UObject
{
	GENERATED_BODY()

	
};
