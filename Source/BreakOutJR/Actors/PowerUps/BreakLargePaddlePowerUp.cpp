// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/PowerUps/BreakLargePaddlePowerUp.h"

#include "Actors/BreakPaddle.h"
#include "Player/BreakPlayer.h"
#include "Actors/BreakBall.h"

void ABreakLargePaddlePowerUp::ActivatePowerUp(AActor* ActivatorActor)
{
	if (!ActivatorActor || !PaddleRef || bIsPowerUpActive) return;

	ABreakBall* BallActor = Cast<ABreakBall>(ActivatorActor);
	if (!BallActor) return;

	if (PaddleRef->GetActivePowerUp() == this) return;

	PaddleRef->ResetWidth();
	PaddleRef->ResetBodyMaxVelocity();
	PaddleRef->SetWidth(125.0f);
	PaddleRef->SetBodyMaxVelocity(PaddleRef->GetBodyMaxVelocity() * 0.80f);

	PaddleRef->SetActivePowerUp(this);
	bIsPowerUpActive = true;
}

void ABreakLargePaddlePowerUp::DeactivatePowerUp()
{
	if (!PaddleRef || !bIsPowerUpActive) return;

	PaddleRef->ResetWidth();
	PaddleRef->ResetBodyMaxVelocity();
	PaddleRef->DeactivatePowerUp();
	bIsPowerUpActive = false;
}

