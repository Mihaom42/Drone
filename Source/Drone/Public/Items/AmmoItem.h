// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/BaseItem.h"
#include "AmmoItem.generated.h"

/**
 * 
 */
UCLASS()
class DRONE_API AAmmoItem : public ABaseItem
{
	GENERATED_BODY()
	
public:
	AAmmoItem();

	//virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

private:
	//UFUNCTION()
	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"), Category = Params)
	int32 AmmoNumber;
};
