// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Actors/BreakPowerUp.h"
#include "BreakLargePaddlePowerUp.generated.h"

UCLASS()
class BREAKOUTJR_API ABreakLargePaddlePowerUp : public ABreakPowerUp
{
	GENERATED_BODY()

public:
	void ActivatePowerUp(AActor* ActivatorActor);
	void DeactivatePowerUp();
};
