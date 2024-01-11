// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Actors/BreakPowerUp.h"
#include "BreakMultiBallPowerUp.generated.h"

/**
 * 
 */
UCLASS()
class BREAKOUTJR_API ABreakMultiBallPowerUp : public ABreakPowerUp
{
	GENERATED_BODY()
	
public:
	void ActivatePowerUp(AActor* ActivatorActor);
	void DeactivatePowerUp();
};
