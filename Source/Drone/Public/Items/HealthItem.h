// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/BaseItem.h"
#include "HealthItem.generated.h"

/**
 * 
 */
UCLASS()
class DRONE_API AHealthItem : public ABaseItem
{
	GENERATED_BODY()
	
public:
	AHealthItem();

	//virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

private:
	//UFUNCTION()
	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"), Category = Params)
	int32 HealthPointsValue;
};
