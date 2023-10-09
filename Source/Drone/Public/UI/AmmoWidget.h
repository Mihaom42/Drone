// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseStateWidget.h"
#include "AmmoWidget.generated.h"

/**
 * 
 */
UCLASS()
class DRONE_API UAmmoWidget : public UBaseStateWidget
{
	GENERATED_BODY()
	
public:
	UAmmoWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
};
