// Copyright Silvergrove Studio

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScheduleManager.generated.h"

UENUM(BlueprintType)
enum class EDaysOfTheWeek : uint8
{
	Monday		UMETA(DisplayName = "Monday"),
	Tuesday		UMETA(DisplayName = "Tuesday"),
	Wednesday	UMETA(DisplayName = "Wednesday"),
	Thursday	UMETA(DisplayName = "Thursday"),
	Friday		UMETA(DisplayName = "Friday"),
	Saturday	UMETA(DisplayName = "Saturday"),
	Sunday		UMETA(DisplayName = "Sunday"),
};

USTRUCT(BlueprintType)
struct FOrderDetails
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int OrderId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDaysOfTheWeek DueDate = EDaysOfTheWeek::Monday;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int> ItemIds = TArray<int>();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* OrderOwner = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int GoldReward = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ReputationReward = 0;
};

UCLASS()
class FORGEBORN_API AScheduleManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AScheduleManager();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FOrderDetails> ActiveOrders;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
