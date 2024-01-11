// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/PowerUps/BreakMultiBallPowerUp.h"

#include "Actors/BreakBall.h"
#include "Player/BreakPlayer.h"

void ABreakMultiBallPowerUp::ActivatePowerUp(AActor* ActivatorActor)
{
	if (!ActivatorActor || !BreakPlayerRef) return;

	ABreakBall* ActivatorBall = Cast<ABreakBall>(ActivatorActor);
	if (!ActivatorBall) return;

	TArray<ABreakBall*> BallsArray = BreakPlayerRef->GetBallsArray();
	const int32 MaxBallsNum = BreakPlayerRef->GetMaxBallsNum();

	if ((BreakPlayerRef->GetBallsPlayingCount() > 1) || BallsArray.Num() == 0) return;

	float BallRadius = ActivatorBall->GetRadius();

	for (int32 idx = 0; idx < MaxBallsNum; idx++)
	{
		int32 SignValue = (idx % 2 == 0) ? 1 : -1;
		FVector NewLocation = ActivatorBall->GetActorLocation() + ActivatorBall->GetActorForwardVector() * SignValue * BallRadius * 4;
		NewLocation -= ActivatorBall->GetActorUpVector() * BallRadius * 2;

		BallsArray[idx]->SetActorLocation(NewLocation);
		BallsArray[idx]->SetIsBodyEnabled(true);
		BallsArray[idx]->SetBodyVelocity(ActivatorBall->GetBodyVelocity());

		BreakPlayerRef->IncreaseBallsPlayingCount();
	}
}

void ABreakMultiBallPowerUp::DeactivatePowerUp()
{
	UE_LOG(LogTemp, Warning, TEXT("Deactivate MultiBall"));
}