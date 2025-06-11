// Copyright Silvergrove Studio

#pragma once

#include "CoreMinimal.h"
#include "Actors/Interaction/WorkstationBase.h"
#include "Characters/PlayerCharacter.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

class UTemperatureComponent;

UCLASS()
class FORGEBORN_API AItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBase();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Items)
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Items)
	TObjectPtr<UTemperatureComponent> TemperatureComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Items)
	FItem Item;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Items)
	EItemQuality FinalItemQuality;
	
	UFUNCTION(BlueprintCallable, Category = Items)
	void SetItemState(EForgeState NewItemState);

	UFUNCTION(BlueprintCallable, Category = Items)
	void SetItem(FItem NewItem);

	UFUNCTION(BlueprintImplementableEvent, Category = Items)
	void OnItemStateChanged(EForgeState NewItemState);

	UFUNCTION(BlueprintImplementableEvent, Category = Items)
	void OnItemTypeChanged(EItems NewItemType);

	UFUNCTION(BlueprintCallable, Category = Items)
	EItemQuality GetItemQuality(float FinalQuality);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
};
