// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BaseStateWidget.h"
#include "Components/TextBlock.h"

UBaseStateWidget::UBaseStateWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UBaseStateWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UBaseStateWidget::UpdateValue(int32 NewValue)
{
	TextValue->SetText(FText::FromString(FString::FromInt(NewValue)));
}