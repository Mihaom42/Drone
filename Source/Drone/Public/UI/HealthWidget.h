// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseStateWidget.h"
#include "HealthWidget.generated.h"

/**
 * 
 */
UCLASS()
class DRONE_API UHealthWidget : public UBaseStateWidget
{
	GENERATED_BODY()

public:
	UHealthWidget(const FObjectInitializer& ObjectInitializer);
	
	virtual void NativeConstruct() override;
};
