// Copyright Silvergrove Studio


#include "Characters/PlayerCharacter.h"

#include "AIController.h"
#include "Actors/Interaction/WorkstationBase.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerStart.h"
#include "GAS/AbilitySystemComponentBase.h"
#include "GAS/AttributeSetBase.h"
#include "Interfaces/InteractInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/DummyPawn.h"
#include "Player/PlayerAIController.h"
#include "Player/PlayerControllerBase.h"
#include "Player/PlayerStateBase.h"
#include "UI/Widgets/MainWidget.h"

APlayerCharacter::APlayerCharacter()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	SpringArm->SetupAttachment(GetRootComponent());
	
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);

	HoldLocation = CreateDefaultSubobject<USceneComponent>("Hold Location");
	HoldLocation->SetupAttachment(GetMesh());

	HammerMesh = CreateDefaultSubobject<USkeletalMeshComponent>("HammerMesh");
	HammerMesh->SetupAttachment(GetMesh());
	HammerMesh->SetHiddenInGame(true);
	
	InteractDistance = 500.f;
	StartupCameraRotation = FRotator::ZeroRotator;
	hasWeapon = false;
	AtGrindingWheel = false;
	AtAnvil = false;
	
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (NewController->IsA(APlayerControllerBase::StaticClass()))
	{
		SetPlayerReferences(NewController);
		InitAbilityActorInfo();
	}
}

FCharacterAttributes APlayerCharacter::GetAttributesSaveData()
{
	FCharacterAttributes PlayerAttributes;
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();

	PlayerAttributes.MaxHealth = ASC->GetNumericAttribute(UAttributeSetBase::GetMaxHealthAttribute());
	PlayerAttributes.Health = ASC->GetNumericAttribute(UAttributeSetBase::GetHealthAttribute());
	PlayerAttributes.Strength = ASC->GetNumericAttribute(UAttributeSetBase::GetStrengthAttribute());
	PlayerAttributes.MaxStrength = ASC->GetNumericAttribute(UAttributeSetBase::GetMaxStrengthAttribute());
	PlayerAttributes.HealthRegen = ASC->GetNumericAttribute(UAttributeSetBase::GetHealthRegenAttribute());
	PlayerAttributes.HealthRegenRate = ASC->GetNumericAttribute(UAttributeSetBase::GetHealthRegenRateAttribute());
	PlayerAttributes.PhysicalResistance = ASC->GetNumericAttribute(UAttributeSetBase::GetPhysicalResistanceAttribute());
	PlayerAttributes.MagicResistance = ASC->GetNumericAttribute(UAttributeSetBase::GetMagicResistanceAttribute());
	PlayerAttributes.WalkSpeed = ASC->GetNumericAttribute(UAttributeSetBase::GetWalkSpeedAttribute());
	PlayerAttributes.MaxWalkSpeed = ASC->GetNumericAttribute(UAttributeSetBase::GetMaxWalkSpeedAttribute());
	PlayerAttributes.SprintSpeed = ASC->GetNumericAttribute(UAttributeSetBase::GetSprintSpeedAttribute());
	PlayerAttributes.MaxSprintSpeed = ASC->GetNumericAttribute(UAttributeSetBase::GetMaxSprintSpeedAttribute());
	PlayerAttributes.LookSpeed = ASC->GetNumericAttribute(UAttributeSetBase::GetLookSpeedAttribute());
	PlayerAttributes.BaseMaxHealth = ASC->GetNumericAttribute(UAttributeSetBase::GetBaseMaxHealthAttribute());
	PlayerAttributes.BaseHealth = ASC->GetNumericAttribute(UAttributeSetBase::GetBaseHealthAttribute());
	PlayerAttributes.BaseMaxStrength = ASC->GetNumericAttribute(UAttributeSetBase::GetBaseMaxStrengthAttribute());
	PlayerAttributes.BaseStrength = ASC->GetNumericAttribute(UAttributeSetBase::GetBaseStrengthAttribute());
	PlayerAttributes.BasePhysicalResistance = ASC->GetNumericAttribute(UAttributeSetBase::GetBasePhysicalResistanceAttribute());
	PlayerAttributes.BaseMagicResistance = ASC->GetNumericAttribute(UAttributeSetBase::GetBaseMagicResistanceAttribute());
	PlayerAttributes.BaseHealthRegen = ASC->GetNumericAttribute(UAttributeSetBase::GetBaseHealthRegenAttribute());
    PlayerAttributes.BaseHealthRegenRate = ASC->GetNumericAttribute(UAttributeSetBase::GetBaseHealthRegenRateAttribute());
	PlayerAttributes.BaseLookSpeed = ASC->GetNumericAttribute(UAttributeSetBase::GetBaseLookSpeedAttribute());
	PlayerAttributes.BaseMaxSprintSpeed = ASC->GetNumericAttribute(UAttributeSetBase::GetBaseMaxSprintSpeedAttribute());
	PlayerAttributes.BaseSprintSpeed = ASC->GetNumericAttribute(UAttributeSetBase::GetBaseSprintSpeedAttribute());
	PlayerAttributes.BaseMaxWalkSpeed = ASC->GetNumericAttribute(UAttributeSetBase::GetBaseMaxWalkSpeedAttribute());
	PlayerAttributes.BaseWalkSpeed = ASC->GetNumericAttribute(UAttributeSetBase::GetBaseWalkSpeedAttribute());
	
	return PlayerAttributes;
}

void APlayerCharacter::Interact()
{
	TArray<AActor*> OverlappedActors;
	GetOverlappingActors(OverlappedActors);

	for (AActor* Actor : OverlappedActors)
	{
		if (Actor && Actor->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
		{
			IInteractInterface::Execute_Interact(Actor);
			break;
		}
	}
}

void APlayerCharacter::AdjustCameraZoom()
{
}

void APlayerCharacter::SetEquippedItem(AItemBase* NewItem)
{
	EquippedItem = NewItem;

	if (EquippedItem)
	{
		hasWeapon = true;
	}
	else
	{
		hasWeapon = false;
	}
}

void APlayerCharacter::PossessAIController()
{
	FActorSpawnParameters SpawnParameters;
	APlayerStart* PlayerStart = Cast<APlayerStart>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass()));

	if (PlayerStart)
	{
		FRotator PlayerStartRotation = PlayerStart->GetActorRotation();
		PossessedDummyPawn = GetWorld()->SpawnActor<ADummyPawn>(TempPawnToPossess, GetActorTransform());
		PossessedDummyPawn->SetActorRotation(PlayerStartRotation);
		PlayerControllerBase->Possess(PossessedDummyPawn);
	}
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AAIController* AIController = GetWorld()->SpawnActor<AAIController>(AAIController::StaticClass(), GetActorLocation(), GetActorRotation(), SpawnParams);
	if (AIController)
	{
		AIController->Possess(this); // Possess the player character
	}
}

void APlayerCharacter::PossessPlayerController(UCommonActivatableWidget* WidgetToRemove)
{
	if (PossessedDummyPawn)
	{
		APlayerStart* PlayerStart = Cast<APlayerStart>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass()));
		
		if (PlayerStart)
		{
			FRotator PlayerStartRotation = PlayerStart->GetActorRotation();
			PlayerControllerBase->isBlendingViewTarget = true;
			PossessedDummyPawn->SetActorLocation(GetActorLocation()); 
			PossessedDummyPawn->SetActorRotation(PlayerStartRotation);
			SetActorRotation(PlayerStartRotation);
			PlayerControllerBase->SetControlRotation(PlayerStartRotation);
			PlayerControllerBase->SetViewTargetWithBlend(this, 1.5, VTBlend_EaseInOut, 4.f);
		 
			FTimerHandle TempHandle;
			GetWorldTimerManager().SetTimer(TempHandle, [this, WidgetToRemove]()
			{
				{
					PlayerControllerBase->isBlendingViewTarget = false;
					PlayerControllerBase->Possess(this);
					PlayerControllerBase->MainWidget->RemoveWidget(WidgetToRemove);
					PlayerControllerBase->MainWidget->SetInputModeMenuClosed();
				
					GetWorldTimerManager().SetTimer(
						CameraRotationTimerHandle,
						this,
						&ThisClass::RotateCameraBackToCenter,
						GetWorld()->GetDeltaSeconds(),
						true
					);
				}
			}, 2.f, false);
		}
	}
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//InteractTrace();
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	AIControllerClass = APlayerAIController::StaticClass();
	AddStartupAbilities();
	StartupCameraRotation = PlayerControllerBase->GetControlRotation();
}

void APlayerCharacter::AddStartupAbilities()
{
	UAbilitySystemComponentBase* ArsenicASC = CastChecked<UAbilitySystemComponentBase>(AbilitySystemComponent);
	ArsenicASC->AddStartupAbilities(StartupAbilities);
}

void APlayerCharacter::InitAbilityActorInfo()
{
	check(PlayerStateBase);
	AbilitySystemComponent = PlayerStateBase->GetAbilitySystemComponent();
	AttributeSet = PlayerStateBase->GetAttributeSet();
	AbilitySystemComponent->InitAbilityActorInfo(PlayerStateBase, this);
	
	Cast<UAbilitySystemComponentBase>(PlayerStateBase->GetAbilitySystemComponent())->AbilityActorInfoSet(); // Inform the ASC that the character is initialized
	
}

void APlayerCharacter::InteractTrace()
{
	AActor* HitActor = PerformInteractLineTrace();

	if (HitActor == LookAtActor) return;

	if (HitActor && HitActor->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
	{
		LookAtActor = HitActor;
		OnPlayerLookAt.Broadcast(HitActor);
		IInteractInterface::Execute_LookAt(HitActor);
	}
	else
	{
		LookAtActor = nullptr;
	}

}

void APlayerCharacter::RotateCameraBackToCenter()
{
	FRotator NewControlRotation = FMath::RInterpTo(PlayerControllerBase->GetControlRotation(), StartupCameraRotation, GetWorld()->GetDeltaSeconds(), 20);
	PlayerControllerBase->SetControlRotation(NewControlRotation);

	if (PlayerControllerBase->GetControlRotation().IsNearlyZero(3.f))
	{
		GetWorldTimerManager().ClearTimer(CameraRotationTimerHandle);
	}
}

AActor* APlayerCharacter::PerformInteractLineTrace()
{
	FVector TraceStart = Camera->GetComponentLocation();
	FVector TraceEnd = TraceStart + (Camera->GetForwardVector() * InteractDistance);

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, CollisionParams);

	return (bHit && HitResult.IsValidBlockingHit() && HitResult.GetActor()) ? HitResult.GetActor() : nullptr;
}

void APlayerCharacter::SetPlayerReferences(AController* NewController)
{
	PlayerControllerBase = Cast<APlayerControllerBase>(NewController);
	PlayerStateBase = GetPlayerState<APlayerStateBase>();
	
	check(PlayerControllerBase);
	check(PlayerStateBase);
}

void APlayerCharacter::HideHammer(bool Hide)
{
	HammerMesh->SetHiddenInGame(Hide);
}
