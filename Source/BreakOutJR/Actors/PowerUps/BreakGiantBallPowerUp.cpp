// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/PowerUps/BreakGiantBallPowerUp.h"

#include "Actors/BreakBall.h"

void ABreakGiantBallPowerUp::ActivatePowerUp(AActor* ActivatorActor)
{
	if (!ActivatorActor) return;

	ActivatorBallRef = Cast<ABreakBall>(ActivatorActor);
	if (!ActivatorBallRef) return;

	if (ActivatorBallRef->GetActivePowerUp() == this) return;

	ActivatorBallRef->ResetRadius();
	ActivatorBallRef->ResetBodyMaxVelocity();
	ActivatorBallRef->SetRadius(135.0f);
	ActivatorBallRef->SetBodyMaxVelocity(ActivatorBallRef->GetBodyMaxVelocity() * 0.80f);

	ActivatorBallRef->SetActivePowerUp(this);
}

void ABreakGiantBallPowerUp::DeactivatePowerUp()
{
	if (!ActivatorBallRef) return;
	ActivatorBallRef->ResetRadius();
	ActivatorBallRef->ResetBodyMaxVelocity();
	ActivatorBallRef = nullptr;
}
