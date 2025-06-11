// Copyright Silvergrove Studio

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TemperatureComponent.generated.h"

enum class EForgeState : uint8;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTemperatureChangedSignature, float, Temperature);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FORGEBORN_API UTemperatureComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTemperatureComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Items)
	float HeatingRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Items)
	float CoolingRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Items)
	float TemperatureChangeAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Items)
	float Temperature;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Items)
	float MaxTemperature;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Items)
	float RequiredTemperatureForShaping;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Items)
	float RequiredTemperatureForRefinement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Items)
	EForgeState TemperatureState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Items)
	float DefaultTemperatureChangeAmount = 7.5;

	UFUNCTION(BlueprintCallable, Category = Items)
	void StartHeating();

	UFUNCTION(BlueprintCallable, Category = Items)
	void StartCooling();

	UFUNCTION(BlueprintCallable, Category = Items)
	void StopTemperatureChangeTimer();

	UFUNCTION(BlueprintCallable)
	void RaiseTemperature();

	UFUNCTION(BlueprintCallable)
	void LowerTemperature();

	UPROPERTY(BlueprintAssignable, Blueprintcallable, Category = Items)
	FOnTemperatureChangedSignature OnTemperatureChanged;

protected:
	virtual void BeginPlay() override;

private:
	FTimerHandle ChangeTemperatureTimerHandle;

	
	
};
