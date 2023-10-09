// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DroneHUD.h"
#include "Components/WidgetComponent.h"
#include "UI/HealthWidget.h"
#include "UI/AmmoWidget.h"
#include "UI/GameOverWidget.h"
#include "UI/VictoryWidget.h"

ADroneHUD::ADroneHUD()
{

}

void ADroneHUD::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();

	if (World == nullptr)
	{
		return;
	}

	if (HealthWidgetClass != nullptr)
	{
		HealthWidget = CreateWidget<UHealthWidget>(World, HealthWidgetClass);
	}

	if (HealthWidget != nullptr)
	{
		HealthWidget->AddToViewport();
	}

	if (AmmoWidgetClass != nullptr)
	{
		AmmoWidget = CreateWidget<UAmmoWidget>(World, AmmoWidgetClass);;
	}

	if (AmmoWidget != nullptr)
	{
		AmmoWidget->AddToViewport();
	}

	if (GameOverWidgetClass != nullptr)
	{
		GameOverWidget = CreateWidget<UGameOverWidget>(World, GameOverWidgetClass);
	}

	if (GameOverWidget != nullptr)
	{
		GameOverWidget->AddToViewport();
		GameOverWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if (VictoryWidgetClass != nullptr)
	{
		VictoryWidget = CreateWidget<UVictoryWidget>(World, VictoryWidgetClass);
	}

	if (VictoryWidget != nullptr)
	{
		VictoryWidget->AddToViewport();
		VictoryWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ADroneHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ADroneHUD::DrawHUD()
{
	Super::DrawHUD();
}

void ADroneHUD::UpdateHealthTextBlockValue(int32 Value)
{
	if (HealthWidget != nullptr)
	{
		HealthWidget->UpdateValue(Value);
	}
}

void ADroneHUD::UpdateAmmoTextBlockValue(int32 Value)
{
	if (AmmoWidget != nullptr)
	{
		AmmoWidget->UpdateValue(Value);
	}
}

void ADroneHUD::ShowGameOverWidget()
{
	if (GameOverWidget != nullptr)
	{
		GameOverWidget->SetVisibility(ESlateVisibility::Visible);
	}

	if (HealthWidget != nullptr)
	{
		HealthWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (AmmoWidget != nullptr)
	{
		AmmoWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ADroneHUD::ShowVictoryWidget()
{
	if (VictoryWidget != nullptr)
	{
		VictoryWidget->SetVisibility(ESlateVisibility::Visible);
	}

	if (HealthWidget != nullptr)
	{
		HealthWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (AmmoWidget != nullptr)
	{
		AmmoWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}
