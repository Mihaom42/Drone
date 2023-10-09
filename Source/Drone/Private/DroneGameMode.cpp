// Fill out your copyright notice in the Description page of Project Settings.


#include "DroneGameMode.h"
#include "Player/DronePawn.h"
#include "Player/DroneController.h"
#include "UI/DroneHUD.h"

ADroneGameMode::ADroneGameMode()
{
	DefaultPawnClass = ADronePawn::StaticClass();
	PlayerControllerClass = ADroneController::StaticClass();
	HUDClass = ADroneHUD::StaticClass();
}