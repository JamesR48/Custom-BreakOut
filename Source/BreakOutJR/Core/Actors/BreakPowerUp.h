// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Actors/BreakCircle.h"
#include "BreakPowerUp.generated.h"

class ABreakPlayer;
class ABreakPaddle;

UCLASS()
class BREAKOUTJR_API ABreakPowerUp : public ABreakCircle
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(Transient)
		ABreakPlayer* BreakPlayerRef = nullptr;

	UPROPERTY(Transient)
		ABreakPaddle* PaddleRef = nullptr;
	
	bool bIsPowerUpActive = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void FindBreakPlayer();
	void FindPlayerPaddle();

public:
	virtual void ActivatePowerUp(AActor* ActivatorActor) PURE_VIRTUAL(ABreakPowerUp::ActivatePowerUp);
	virtual void DeactivatePowerUp() PURE_VIRTUAL(ABreakPowerUp::DeactivatePowerUp);

	FORCEINLINE bool GetIsPowerUpActive() { return bIsPowerUpActive; }
};
