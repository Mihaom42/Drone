// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseStateWidget.generated.h"

/**
 * 
 */
UCLASS()
class DRONE_API UBaseStateWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UBaseStateWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	virtual void UpdateValue(int32 NewValue);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextValue;
};
