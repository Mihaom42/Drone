// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VictoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class DRONE_API UVictoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UVictoryWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* VictoryText;
};
