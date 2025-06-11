// Copyright Silvergrove Studio


#include "Actors/ResourceManager.h"

#include "Kismet/GameplayStatics.h"
#include "Player/PlayerControllerBase.h"

// Sets default values
AResourceManager::AResourceManager()
{
	Ore = 0;
	Ingot = 0;
	Wood = 0;
	Coal = 0;
	Gold = 0;
}

void AResourceManager::UpdateOre(int Amount)
{
	Ore += Amount;
	OnOreChanged();
}

void AResourceManager::UpdateIngot(int Amount)
{
	Ingot += Amount;
	OnIngotChanged();
}

void AResourceManager::UpdateWood(int Amount)
{
	Wood += Amount;
	OnWoodChanged();
}

void AResourceManager::UpdateCoal(int Amount)
{
	Coal += Amount;
	OnCoalChanged();
}

void AResourceManager::UpdateGold(int Amount)
{
	Gold += Amount;
	OnGoldChanged();
}

void AResourceManager::BeginPlay()
{
	Super::BeginPlay();

	PlayerControllerBase = Cast<APlayerControllerBase>(UGameplayStatics::GetPlayerController(GetWorld(),0));
}


