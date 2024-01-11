// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/PowerUps/BreakTinyBallPowerUp.h"

#include "Actors/BreakBall.h"

void ABreakTinyBallPowerUp::ActivatePowerUp(AActor* ActivatorActor)
{
	if (!ActivatorActor) return;

	ActivatorBallRef = Cast<ABreakBall>(ActivatorActor);
	if (!ActivatorBallRef) return;

	if (ActivatorBallRef->GetActivePowerUp() == this) return;

	ActivatorBallRef->ResetRadius();
	ActivatorBallRef->ResetBodyMaxVelocity();
	ActivatorBallRef->SetRadius(70.0f);
	ActivatorBallRef->SetBodyMaxVelocity(ActivatorBallRef->GetBodyMaxVelocity() * 1.25f);

	ActivatorBallRef->SetActivePowerUp(this);
}


void ABreakTinyBallPowerUp::DeactivatePowerUp()
{
	if (!ActivatorBallRef) return;
	ActivatorBallRef->ResetRadius();
	ActivatorBallRef->ResetBodyMaxVelocity();
	ActivatorBallRef = nullptr;
}
