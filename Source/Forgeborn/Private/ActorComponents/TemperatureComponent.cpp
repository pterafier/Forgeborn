// Copyright Silvergrove Studio


#include "ActorComponents/TemperatureComponent.h"

#include "Actors/Interaction/WorkstationBase.h"

UTemperatureComponent::UTemperatureComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	Temperature = 0.0f;
	MaxTemperature = 2500.0f;
	HeatingRate = 0.25f;
	CoolingRate = 0.12f;
	TemperatureChangeAmount = 5.0f;
	RequiredTemperatureForRefinement = 2000.f;
	RequiredTemperatureForShaping = 1500.f;
	TemperatureState = EForgeState::Cooling;
}


void UTemperatureComponent::StartHeating()
{
	GetWorld()->GetTimerManager().SetTimer(ChangeTemperatureTimerHandle, this, &ThisClass::RaiseTemperature, HeatingRate, true);

	TemperatureState = EForgeState::Heating;
}

void UTemperatureComponent::StartCooling()
{
	GetWorld()->GetTimerManager().SetTimer(ChangeTemperatureTimerHandle, this, &ThisClass::LowerTemperature, CoolingRate, true);

	TemperatureState = EForgeState::Cooling;
}

void UTemperatureComponent::StopTemperatureChangeTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(ChangeTemperatureTimerHandle);
}

void UTemperatureComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OnTemperatureChanged.Broadcast(Temperature);
}

void UTemperatureComponent::RaiseTemperature()
{
	Temperature += TemperatureChangeAmount;
	Temperature = FMath::Clamp(Temperature, 0, MaxTemperature);

	OnTemperatureChanged.Broadcast(Temperature);
}

void UTemperatureComponent::LowerTemperature()
{
	Temperature -= TemperatureChangeAmount;
	Temperature = FMath::Clamp(Temperature, 0, MaxTemperature);
	
	OnTemperatureChanged.Broadcast(Temperature);
}

