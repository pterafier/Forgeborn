// Copyright Silvergrove Studio

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResourceManager.generated.h"

class APlayerControllerBase;

UCLASS()
class FORGEBORN_API AResourceManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AResourceManager();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<APlayerControllerBase> PlayerControllerBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Ore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Ingot;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Wood;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Coal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Gold;

	UFUNCTION(BlueprintCallable)
	void UpdateOre(int Amount);

	UFUNCTION(BlueprintCallable)
	void UpdateIngot(int AmountToAdd);

	UFUNCTION(BlueprintCallable)
	void UpdateWood(int AmountToAdd);

	UFUNCTION(BlueprintCallable)
	void UpdateCoal(int AmountToAdd);

	UFUNCTION(BlueprintCallable)
	void UpdateGold(int AmountToAdd);

	UFUNCTION(BlueprintImplementableEvent)
	void OnOreChanged();

	UFUNCTION(BlueprintImplementableEvent)
	void OnIngotChanged();

	UFUNCTION(BlueprintImplementableEvent)
	void OnWoodChanged();

	UFUNCTION(BlueprintImplementableEvent)
	void OnCoalChanged();

	UFUNCTION(BlueprintImplementableEvent)
	void OnGoldChanged();

protected:
	virtual void BeginPlay() override;
	
};
