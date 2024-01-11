// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/BreakTextDisplayerWidget.h"
#include "Components/TextBlock.h"

void UBreakTextDisplayerWidget::SetTextToDisplay(FString NewText)
{
	TextToDisplay->SetText(FText::FromString(NewText));
}
