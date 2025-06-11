// Copyright Silvergrove Studio


#include "Actors/Checkpoint.h"

#include "Characters/PlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "Game/Systems/Save/SaveGameSubsystem.h"
#include "Kismet/GameplayStatics.h"

ACheckpoint::ACheckpoint()
{
	PrimaryActorTick.bCanEverTick = true;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>("TriggerBox");
	TriggerBox->SetupAttachment(GetRootComponent());
}

void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnTriggerBoxBeginOverlap);
}

void ACheckpoint::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	TriggerBox->OnComponentBeginOverlap.RemoveDynamic(this, &ThisClass::OnTriggerBoxBeginOverlap);
}

void ACheckpoint::OnTriggerBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != UGameplayStatics::GetPlayerCharacter(this, 0)) return;
	
	USaveGameSubsystem* SaveSubsystem = USaveGameSubsystem::Get(this);
	if (!SaveSubsystem) return;
	if (!SaveSubsystem->CanSave) return; // helps prevent an unintentional save when the player transform is updated at begin play
	
	ISaveGameInterface* SaveGameInterface = SaveSubsystem;
	FetchSaveData();
	SaveGameInterface->Execute_SetPlayerSaveData(SaveSubsystem, PlayerSaveData);
	SaveGameInterface->Execute_SetPlayerAttributes(SaveSubsystem, PlayerAttributes);
	SaveGameInterface->Execute_SaveGameData(SaveSubsystem);
}

void ACheckpoint::FetchSaveData()
{
	APlayerStateBase* ArsenicPlayerState = Cast<APlayerStateBase>(UGameplayStatics::GetPlayerState(this, 0));
	APlayerCharacter* ArsenicPlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	PlayerSaveData = ArsenicPlayerState->GetPlayerSaveData();
	PlayerAttributes = ArsenicPlayerCharacter->GetAttributesSaveData();
}

