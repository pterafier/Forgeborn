// Copyright Silvergrove Studio

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "GameFramework/Actor.h"
#include "Player/PlayerStateBase.h"
#include "Checkpoint.generated.h"

class UBoxComponent;

UCLASS()
class FORGEBORN_API ACheckpoint : public AActor
{
	GENERATED_BODY()
	
public:	
	ACheckpoint();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger")
	TObjectPtr<UBoxComponent> TriggerBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Save")
	FPlayerSave PlayerSaveData;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Save")
	FCharacterAttributes PlayerAttributes;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void OnTriggerBoxBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION(BlueprintCallable, Category = "Save")
	void FetchSaveData();
};
