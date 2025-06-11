// Copyright Silvergrove Studio


#include "Player/DummyPawn.h"

// Sets default values
ADummyPawn::ADummyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void ADummyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADummyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADummyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

