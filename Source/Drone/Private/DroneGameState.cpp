// Fill out your copyright notice in the Description page of Project Settings.


#include "DroneGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Turret/TurretPawn.h"
#include "Engine/World.h"
#include "UI/DroneHUD.h"

ADroneGameState::ADroneGameState()
{

}

void ADroneGameState::BeginPlay()
{
	NumberOfTurret = GetNumberOfTurrets();
}

void ADroneGameState::Tick(float DeltaSeconds)
{

}

int32 ADroneGameState::GetNumberOfTurrets()
{
	TSubclassOf<AActor> ClassToFind = ATurretPawn::StaticClass();
	TArray<AActor*> FoundActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToFind, FoundActors);
	
	return FoundActors.Num();
}

void ADroneGameState::AnnounceVictory()
{
	UWorld* World = GetWorld();

	if (World == nullptr)
	{
		return;
	}

	ADroneHUD* DroneHUD = Cast<ADroneHUD>(World->GetFirstPlayerController()->GetHUD());

	if (DroneHUD != nullptr)
	{
		DroneHUD->ShowVictoryWidget();
	}
}


void ADroneGameState::UpdateNumberOfTurrets()
{
	NumberOfTurret = GetNumberOfTurrets();

	//TODO: It it necessary to make search why NumberOfTurrnt does not decrease after killing a first turret
	if ((NumberOfTurret - 1) == 0)
	{
		AnnounceVictory();
	}
}
