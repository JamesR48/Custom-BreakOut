// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BreakTextDisplayerWidget.generated.h"

class UTextBlock;

UCLASS()
class BREAKOUTJR_API UBreakTextDisplayerWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UTextBlock* TextToDisplay;

public:
	void SetTextToDisplay(FString NewText);

};
