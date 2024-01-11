// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Actors/BreakPowerUp.h"
#include "Kismet/GameplayStatics.h"

#include "Player/BreakPlayer.h"
#include "Actors/BreakPaddle.h"
#include "Actors/BreakBall.h"

// Called when the game starts or when spawned
void ABreakPowerUp::BeginPlay()
{
	Super::BeginPlay();
	
	FindBreakPlayer();

	this->CollisionResponse = ECollisionResponseType::ECR_OVERLAP;
	this->PhysicsBehaviour = EPhysicsBehaviourType::EPB_STATIC;
	this->SetIsBodyEnabled(false);
}

void ABreakPowerUp::FindBreakPlayer()
{
	AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ABreakPlayer::StaticClass());
	if (!FoundActor) return;

	BreakPlayerRef = Cast<ABreakPlayer>(FoundActor);
	if (!BreakPlayerRef) return;

	BreakPlayerRef->AllBallsLostDel.AddUObject(this, &ABreakPowerUp::DeactivatePowerUp);

	FindPlayerPaddle();
}

void ABreakPowerUp::FindPlayerPaddle()
{
	if (!BreakPlayerRef) return;

	PaddleRef = BreakPlayerRef->GetBreakPaddleRef();
}

