// Copyright Silvergrove Studio

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AudioSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class FORGEBORN_API UAudioSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio Subsystem")
	float MasterVolume = 0.7f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio Subsystem")
	float MusicVolume = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio Subsystem")
	float SoundEffectsVolume = 0.75f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio Subsystem")
	float AmbientSoundsVolume = 0.5f;
	
};
