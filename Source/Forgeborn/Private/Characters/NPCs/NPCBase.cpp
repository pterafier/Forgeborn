// Copyright Silvergrove Studio


#include "Characters/NPCs/NPCBase.h"

#include "Animation/WidgetAnimation.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Widgets/HUD/Interaction/InteractPopup.h"

ANPCBase::ANPCBase()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetupAttachment(GetRootComponent());

	PopupWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("Popup Widget");
	PopupWidgetComponent->SetupAttachment(GetRootComponent());
	PopupWidgetComponent->SetHiddenInGame(true);

	OrderSize = 0;
	CustomerType = ECustomerType::None;
	GoldBonus = 0;
	WaitTime = 30.0f;
}

void ANPCBase::HidePopupWidget()
{
	if (UInteractPopup* PopupWidget = Cast<UInteractPopup>(PopupWidgetComponent->GetWidget()))
	{
		float AnimationDuration = PopupWidget->AnimateOut->GetEndTime();
		
		PopupWidget->PlayAnimation(PopupWidget->AnimateOut);
		
		FTimerHandle TempHandle;
		GetWorldTimerManager().SetTimer(TempHandle, [this]()
		{
			{
				PopupWidgetComponent->SetHiddenInGame(false);
			}
		}, AnimationDuration, false);
	}
}

void ANPCBase::ShowPopupWidget()
{
	PopupWidgetComponent->SetHiddenInGame(false);
	if (UInteractPopup* PopupWidget = Cast<UInteractPopup>(PopupWidgetComponent->GetWidget()))
	{
		PopupWidget->PlayAnimation(PopupWidget->AnimateIn);
	}
}

void ANPCBase::Interact_Implementation()
{
}

void ANPCBase::LookAt_Implementation()
{
}

void ANPCBase::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereBeginOverlap_InteractSphere);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnSphereEndOverlap_InteractSphere);
}

void ANPCBase::OnSphereBeginOverlap_InteractSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != UGameplayStatics::GetPlayerCharacter(this, 0)) return;

	ShowPopupWidget();
}

void ANPCBase::OnSphereEndOverlap_InteractSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != UGameplayStatics::GetPlayerCharacter(this, 0)) return;

	HidePopupWidget();
}
