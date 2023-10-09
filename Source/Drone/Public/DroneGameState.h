// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "DroneGameState.generated.h"

/**
 * 
 */
UCLASS()
class DRONE_API ADroneGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ADroneGameState();

	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaSeconds) override;

	void UpdateNumberOfTurrets();

	int32 NumberOfTurret;

private:
	int32 GetNumberOfTurrets();

	void AnnounceVictory();
};
