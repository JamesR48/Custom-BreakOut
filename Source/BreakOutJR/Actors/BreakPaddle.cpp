// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/BreakPaddle.h"
#include "BreakPaddle.h"

void ABreakPaddle::MoveRight(float AxisValue)
{
	Velocity = FVector::ForwardVector * PaddleMoveSpeed * AxisValue;
	if (AxisValue == 0.0f) return;

	float LocationX = FMath::Clamp(GetActorLocation().X, BoxWidth - 710.0f, 710.0f - BoxWidth);
	SetActorLocation(FVector(LocationX, 0.0f, GetActorLocation().Z));
}

void ABreakPaddle::SetActivePowerUp(ABreakPowerUp* NewPowerUp)
{
	if (CurrentActivePowerUp && CurrentActivePowerUp == NewPowerUp) return;

	CurrentActivePowerUp = NewPowerUp;
}

void ABreakPaddle::DeactivatePowerUp()
{
	if (!CurrentActivePowerUp) return;
	CurrentActivePowerUp = nullptr;
	ResetWidth();
	ResetBodyMaxVelocity();
	PaddleBouncesCount = 0;
}
