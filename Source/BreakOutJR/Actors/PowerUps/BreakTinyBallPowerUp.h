// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Actors/BreakPowerUp.h"
#include "BreakTinyBallPowerUp.generated.h"

class ABreakBall;

UCLASS()
class BREAKOUTJR_API ABreakTinyBallPowerUp : public ABreakPowerUp
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(Transient)
		ABreakBall* ActivatorBallRef = nullptr;

public:
	void ActivatePowerUp(AActor* ActivatorActor);
	void DeactivatePowerUp();
};
