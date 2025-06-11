// Copyright Silvergrove Studio


#include "Actors/Interaction/WorkstationBase.h"

#include "AIController.h"
#include "AITypes.h"
#include "Animation/WidgetAnimation.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Characters/PlayerCharacter.h"
#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"
#include "Player/PlayerControllerBase.h"
#include "Runtime/CinematicCamera/Public/CineCameraComponent.h"
#include "UI/Widgets/MainWidget.h"
#include "UI/Widgets/HUD/Interaction/InteractPopup.h"

AWorkstationBase::AWorkstationBase()
{
	PrimaryActorTick.bCanEverTick = true;

	ActorRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = ActorRoot;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh Component");
	StaticMeshComponent->SetupAttachment(GetRootComponent());

	SphereCollision = CreateDefaultSubobject<USphereComponent>("Sphere Collision");
	SphereCollision->SetupAttachment(StaticMeshComponent);
	
	AnimationTargetLocation = CreateDefaultSubobject<USphereComponent>("Animation Target");
	AnimationTargetLocation->SetupAttachment(GetRootComponent());

	WorkstationCamera = CreateDefaultSubobject<UCineCameraComponent>("Workstation Camera");
	WorkstationCamera->SetupAttachment(GetRootComponent());

	Arrow = CreateDefaultSubobject<UArrowComponent>("Arrow");
	Arrow->SetupAttachment(StaticMeshComponent);

	PopupWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("Popup Widget");
	PopupWidgetComponent->SetupAttachment(GetRootComponent());

	WorkstationType = EWorkstationType::None;
	WorkstationAction = EWorkstationAction::None;
	CameraBlendExp = 5.0f;
	isInteractable = true;
	hasMinigame = false;
	isAcceptingItems = false;
	isLocked = false;
	ShouldLock = false;
}

void AWorkstationBase::Interact_Implementation()
{
	if (!isInteractable) return;

	APlayerControllerBase* PlayerController = PlayerCharacter->PlayerControllerBase;
	
	if (hasMinigame && !PlayerController->isBlendingViewTarget)
	{
		isInteractable = false;

		float DistanceToAnimLocation = FVector::Dist(PlayerCharacter->GetActorLocation(), AnimationTargetLocation->GetComponentLocation());
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Cyan, FString::Printf(TEXT("Distance: %f"), DistanceToAnimLocation));

		if (DistanceToAnimLocation <= 110.f)
		{
			SetPlayerAtWorkstation();
			PlayerCharacter->PossessAIController();
			BlendViewToWorkstation();
			SnapToWorkstation();
		}
		else
		{
			SetPlayerAtWorkstation();
			PlayerCharacter->PossessAIController();
			BlendViewToWorkstation();
			
			AAIController* AIController = Cast<AAIController>(PlayerCharacter->GetController());
			if (AIController)
			{
				FAIMoveRequest MoveRequest;
				MoveRequest.SetGoalLocation(AnimationTargetLocation->GetComponentLocation());
				MoveRequest.SetAcceptanceRadius(3.f);
				AIController->MoveTo(MoveRequest);

				AIController->ReceiveMoveCompleted.AddDynamic(this, &ThisClass::OnAIMoveToComplete);
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "No AI Controller found");
			}
		}
		
		HidePopupWidget();
	}
}

void AWorkstationBase::LookAt_Implementation()
{
}

void AWorkstationBase::HidePopupWidget()
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

void AWorkstationBase::ShowPopupWidget()
{
	PopupWidgetComponent->SetHiddenInGame(false);
	if (UInteractPopup* PopupWidget = Cast<UInteractPopup>(PopupWidgetComponent->GetWidget()))
	{
		PopupWidget->PlayAnimation(PopupWidget->AnimateIn);
	}
}

void AWorkstationBase::SetWorkstationLocked(bool Locked)
{
	isLocked = Locked;
}

void AWorkstationBase::PlayerExitWorkstation()
{
	if (!hasMinigame) return;

	isInteractable = true;
	ShowPopupWidget();

	PlayerCharacter->PossessPlayerController(MinigameWidgetInstance);

	APlayerControllerBase* PlayerController = PlayerCharacter->PlayerControllerBase;
	PlayerController->SetPlayerInMinigame(false);
	PlayerController->EnableInput(PlayerController);
	PlayerController->SetCurrentWorkstation(nullptr);

	OnPlayerExitWorkstation();
}

void AWorkstationBase::BeginPlay()
{
	Super::BeginPlay();

	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereBeginOverlap_InteractSphere);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnSphereEndOverlap_InteractSphere);
	PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void AWorkstationBase::OnSphereBeginOverlap_InteractSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != PlayerCharacter) return;

	ShowPopupWidget();
}

void AWorkstationBase::OnSphereEndOverlap_InteractSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != PlayerCharacter) return;

	HidePopupWidget();
}

void AWorkstationBase::OnSphereBeginOverlap_AnimationSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//HidePopupWidget();
}

void AWorkstationBase::OnSphereEndOverlap_AnimationSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AWorkstationBase::OnAIMoveToComplete(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	SnapToWorkstation();
}

void AWorkstationBase::SnapToWorkstation()
{
	APlayerControllerBase* PlayerController = PlayerCharacter->PlayerControllerBase;
	PlayerController->SetPlayerInMinigame(true);
	PlayerCharacter->SetActorLocation(AnimationTargetLocation->GetComponentLocation());
	PlayerCharacter->GetMesh()->SetWorldRotation(Arrow->GetComponentRotation());
}

void AWorkstationBase::BlendViewToWorkstation()
{
	APlayerControllerBase* PlayerController = PlayerCharacter->PlayerControllerBase;
	PlayerController->isBlendingViewTarget = true;
	PlayerController->SetViewTargetWithBlend(this, 1.5, VTBlend_EaseInOut, CameraBlendExp, false);
	
	FTimerHandle TempHandle;
	GetWorldTimerManager().SetTimer(TempHandle, [this]()
	{
		{
			APlayerControllerBase* PlayerController = PlayerCharacter->PlayerControllerBase;
			PlayerController->isBlendingViewTarget = false;
			
			UCommonActivatableWidgetContainerBase* TargetStack = PlayerController->MainWidget->Popup_Stack;
			MinigameWidgetInstance = PlayerController->MainWidget->PushWidgetToTargetStack(TargetStack, MinigameWidgetClass, false, false);
			OnMinigameWidgetActivated();
		}
	}, 2.f, false);
}

void AWorkstationBase::SetPlayerAtWorkstation()
{
	APlayerControllerBase* PlayerController = PlayerCharacter->PlayerControllerBase;
	PlayerController->DisableInput(PlayerController);
	PlayerController->SetCurrentWorkstation(this);
}
