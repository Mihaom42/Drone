// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DroneHUD.generated.h"

/**
 * 
 */
UCLASS()
class DRONE_API ADroneHUD : public AHUD
{
	GENERATED_BODY()

public:
	ADroneHUD();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void DrawHUD() override;

public:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"), Category = Widgets)
	TSubclassOf<class UBaseStateWidget> HealthWidgetClass;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"), Category = Widgets)
	TSubclassOf<class UBaseStateWidget> AmmoWidgetClass;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"), Category = Widgets)
	TSubclassOf<class UGameOverWidget> GameOverWidgetClass;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"), Category = Widgets)
	TSubclassOf<class UVictoryWidget> VictoryWidgetClass;

	void UpdateHealthTextBlockValue(int32 Value);

	void UpdateAmmoTextBlockValue(int32 Value);

	void ShowGameOverWidget();

	void ShowVictoryWidget();

private:
	class UHealthWidget* HealthWidget;

	class UAmmoWidget* AmmoWidget;

	class UGameOverWidget* GameOverWidget;
	
	class UVictoryWidget* VictoryWidget;;
};
