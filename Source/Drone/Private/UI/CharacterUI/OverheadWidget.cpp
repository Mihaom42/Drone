// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CharacterUI/OverheadWidget.h"
#include "Components/TextBlock.h"

void UOverheadWidget::NativeDestruct()
{
	RemoveFromParent();
	Super::NativeDestruct();
}

void UOverheadWidget::SetDisplayText(FString TextToDisplay)
{
	if (DisplayText)
	{
		DisplayText->SetText(FText::FromString(TextToDisplay));
	}
}

void UOverheadWidget::ShowPlayerNetRole(APawn* InPawn)
{
	ENetRole LocalRole = InPawn->GetLocalRole();
	FString Role;
	switch (LocalRole)
	{
	case ROLE_None:
		Role = FString("None");
		break;
	case ROLE_SimulatedProxy:
		Role = FString("Simulated Proxy");
		break;
	case ROLE_AutonomousProxy:
		Role = FString("Autonomous Proxy");
		break;
	case ROLE_Authority:
		Role = FString("Authority");
		break;
	default:
		break;
	}

	FString LocalRoleString = FString::Printf(TEXT("Local Role: %s"), *Role);
	SetDisplayText(LocalRoleString);
}