// Copyright Silvergrove Studio


#include "Actors/Items/ItemBase.h"

#include "ActorComponents/TemperatureComponent.h"

AItemBase::AItemBase()
{
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	RootComponent = SkeletalMeshComponent;

	TemperatureComponent = CreateDefaultSubobject<UTemperatureComponent>("Temperature Component");
	
	Item = FItem();
}

void AItemBase::SetItemState(EForgeState NewItemState)
{
	Item.ForgeState = NewItemState;
}

void AItemBase::SetItem(FItem NewItem)
{
	Item = NewItem;
	TemperatureComponent->MaxTemperature = Item.MaxTemperature;
	TemperatureComponent->RequiredTemperatureForShaping = Item.TargetTransformationTemperature;
	TemperatureComponent->RequiredTemperatureForRefinement = Item.TargetTransformationTemperature;
}

#if 0
	if (Item.Type == ECraftableItems::Ore && Temperature >= RequiredTemperatureForRefinement)
	{
		SetItem(ECraftableItems::IronIngot);
		OnItemTypeChanged(ECraftableItems::IronIngot);

		Temperature = 0.0f;
		SetItemState(EForgeState::Cool);
	}
	else if (Item.Type == ECraftableItems::IronIngot && Temperature >= RequiredTemperatureForShaping)
	{
		SetItemState(EForgeState::Hot);
		OnItemStateChanged(EForgeState::Hot);
	}
#endif

EItemQuality AItemBase::GetItemQuality(float FinalQuality)
{
	if (FinalQuality <= 0.0f)
	{
		return EItemQuality::Poor;		
	}
	if (FinalQuality >= 0.0f && FinalQuality < 1.0f)
	{
		return EItemQuality::Poor;		
	}
	if (FinalQuality > 1.0f && FinalQuality < 2.0f)
	{
		return EItemQuality::Fair;
	}
	if (FinalQuality > 2.0f && FinalQuality < 3.0f)
	{
		return EItemQuality::Good;
	}
	if (FinalQuality > 3.0f && FinalQuality < 4.0f)
	{
		return EItemQuality::Excellent;
	}
	if (FinalQuality > 4.0f)
	{
		return EItemQuality::Legendary;
	}

	return EItemQuality::Good;
}

void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

