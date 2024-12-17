// Fill out your copyright notice in the Description page of Project Settings.


#include "DroneGameMode.h"
#include "Player/DronePawn.h"
#include "Player/DroneController.h"
#include "UI/DroneHUD.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"

ADroneGameMode::ADroneGameMode()
{
	DefaultPawnClass = ADronePawn::StaticClass();
	PlayerControllerClass = ADroneController::StaticClass();
	HUDClass = ADroneHUD::StaticClass();
}

void ADroneGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (GameState)
	{
		int32 NumberOfPlayers = GameState->PlayerArray.Num();
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				1,
				600.f,
				FColor::Yellow,
				FString::Printf(TEXT("Players in game: %d"), NumberOfPlayers)
			);

			APlayerState* PlayerState = NewPlayer->GetPlayerState<APlayerState>();
			if (PlayerState)
			{
				FString PlayerName = PlayerState->GetPlayerName();
				GEngine->AddOnScreenDebugMessage(
					-1,
					60.f,
					FColor::Green,
					FString::Printf(TEXT("%s has joined the game!"), *PlayerName)
				);	
			}
		}
	}
}

void ADroneGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	if (GameState == nullptr)
	{
		return;
	}

	APlayerState* PlayerState = Exiting->GetPlayerState<APlayerState>();
	
	if (PlayerState == nullptr)
	{
		return;
	}

	int32 NumberOfPlayers = GameState->PlayerArray.Num();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
				1,
				600.f,
				FColor::Yellow,
				FString::Printf(TEXT("Players in game: %d"), NumberOfPlayers - 1)
			);

		FString PlayerName = PlayerState->GetPlayerName();
		GEngine->AddOnScreenDebugMessage(
			-1,
			60.f,
			FColor::Red,
			FString::Printf(TEXT("%s has left the game!"), *PlayerName)
		);	
	}
	
}
