// Copyright Silvergrove Studio


#include "Actors/ScheduleManager.h"

// Sets default values
AScheduleManager::AScheduleManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AScheduleManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AScheduleManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

