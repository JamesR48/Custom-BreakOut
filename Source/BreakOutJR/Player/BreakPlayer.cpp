// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BreakPlayer.h"
#include "Kismet/GameplayStatics.h"

#include "Actors/BreakPaddle.h"
#include "Actors/BreakBall.h"
#include "Core/Actors/BreakBoxVolume.h"
#include "General/BreakPhysicsManager.h"
#include "General/BreakGameStateBase.h"
#include "Components/BreakHealthComponent.h"

// Sets default values
ABreakPlayer::ABreakPlayer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerHealthComponent = CreateDefaultSubobject<UBreakHealthComponent>(TEXT("PlayerHealthComponent"));
}

// Called when the game starts or when spawned
void ABreakPlayer::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABreakPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABreakPlayer::MovePaddleRight(float AxisValue)
{
	if (!PaddleRef) return;

	PaddleRef->MoveRight(AxisValue);
}

void ABreakPlayer::LaunchBall()
{
	if ( (!PaddleRef && BallsArray.Num() == 0) || !bIsBallOnPaddle) return;

	DetachBallFromPaddle();

	FVector LaunchForce = FVector(PaddleRef->GetBodyVelocity().X, 0.0f, LaunchMagnitude);
	BallsArray[0]->ApplyBodyImpulse(LaunchForce);
}

void ABreakPlayer::SetupBreakPlayer()
{
	FindBoxVolumeBounds();
	SpawnPaddle();
	SpawnAllBalls();
}

void ABreakPlayer::FindBoxVolumeBounds()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABreakBoxVolume::StaticClass(), FoundActors);
	if (FoundActors.Num() == 0) return;

	for (AActor* VolumeActor : FoundActors)
	{
		ABreakBoxVolume* BoxVolume = Cast<ABreakBoxVolume>(VolumeActor);
		if (!BoxVolume || !BreakPhysicsManagerRef) return;

		BreakPhysicsManagerRef->AddPhysicsBody(VolumeActor);
		BoxVolume->IsCollidingDel.BindUObject(this, &ABreakPlayer::BallIsLost);
		BoxVolumeBoundsArray.Add(BoxVolume);
	}
}

void ABreakPlayer::SpawnPaddle()
{
	if (!PaddleBPClass || !BreakPhysicsManagerRef) return;

	PaddleRef = GetWorld()->SpawnActor<ABreakPaddle>(PaddleBPClass, FVector::ZeroVector, FRotator::ZeroRotator);

	if (!PaddleRef) return;
	BreakPhysicsManagerRef->AddPhysicsBody( (AActor*)PaddleRef );

	PaddleRef->IsCollidingDel.BindUObject(this, &ABreakPlayer::PaddleBouncesEvents);
}

void ABreakPlayer::SpawnAllBalls()
{
	if (!BallBPClass || !PaddleRef || !BreakPhysicsManagerRef) return;

	BallSpawningLocation = PaddleRef->GetActorLocation() + (GetActorUpVector() * PaddleRef->GetHeight()*3.0f);

	for(int32 idx = 0; idx < MaxBallsNum; idx++)
	{
		ABreakBall* Ball = GetWorld()->SpawnActor<ABreakBall>(BallBPClass, BallSpawningLocation, FRotator::ZeroRotator);
		if (!Ball) continue;

		Ball->SetIsBodyEnabled(false);
		BreakPhysicsManagerRef->AddPhysicsBody((AActor*)Ball);
		BallsArray.Add(Ball);
	}

	AttachBallToPaddle(BallsArray[0]);
}

void ABreakPlayer::AttachBallToPaddle(ABreakBall* BallToAttach)
{
	if (!BallToAttach) return;

	if (BallToAttach->GetActorLocation() != BallSpawningLocation)
	{
		BallToAttach->SetActorLocation(BallSpawningLocation);
	}

	if (!BallToAttach->GetIsBodyEnabled()) BallToAttach->SetIsBodyEnabled(true);

	const FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, true);
	BallToAttach->AttachToActor(PaddleRef, AttachmentTransformRules);
	bIsBallOnPaddle = true;

	BallsPlayingCount++;
	BallsPlayingCount = FMath::Clamp(BallsPlayingCount, 0, MaxBallsNum);
}

void ABreakPlayer::DetachBallFromPaddle()
{
	if (BallsArray.Num() == 0) return;

	const FDetachmentTransformRules DetachmentTransformRules = FDetachmentTransformRules(EDetachmentRule::KeepWorld, false);
	BallsArray[0]->DetachFromActor(DetachmentTransformRules);
	bIsBallOnPaddle = false;
}

void ABreakPlayer::BallIsLost(AActor* BoxVolumeActor, AActor* BallActor)
{
	if (!BreakGameStateRef || BreakGameStateRef->GetIsGameOver()) return;

	ABreakBall* BallPhysics = Cast<ABreakBall>(BallActor);
	if (!BallPhysics) return;

	BallPhysics->SetBallIsLost(true);
	BallPhysics->SetIsBodyEnabled(false);
	BallPhysics->DeactivatePowerUp();

	BallsPlayingCount--;
	BallsPlayingCount = FMath::Clamp(BallsPlayingCount, 0, MaxBallsNum);

	PlayCameraShake(OnBallLostCameraShakeScale);

	if (BallsPlayingCount <= 0)
	{
		if (PlayerHealthComponent) PlayerHealthComponent->DecreaseHealth();
			
		AllBallsLostDel.Broadcast();

		AttachBallToPaddle(BallsArray[0]);
		
		PaddleRef->ResetWidth();
		PaddleRef->ResetBodyMaxVelocity();
	}
}

void ABreakPlayer::PaddleBouncesEvents(AActor* PaddleActor, AActor* BallActor)
{
	ABreakBall* BallPhysics = Cast<ABreakBall>(BallActor);
	if (!BallPhysics) return;

	if (BallPhysics->GetActivePowerUp())
	{
		BallPhysics->UpdatePaddleBouncesCount();
		
		if (BallPhysics->GetPaddleBouncesCount() >= 2)
		{
			BallPhysics->SetPaddleBouncesCount(0);
			BallPhysics->DeactivatePowerUp();
		}
	}

	if (PaddleRef->GetActivePowerUp())
	{
		PaddleRef->UpdatePaddleBouncesCount();

		if (PaddleRef->GetPaddleBouncesCount() >= 2)
		{
			PaddleRef->SetPaddleBouncesCount(0);
			PaddleRef->DeactivatePowerUp();
		}
	}
}

void ABreakPlayer::IncreaseBallsPlayingCount()
{
	BallsPlayingCount++;
	BallsPlayingCount = FMath::Clamp(BallsPlayingCount, 0, MaxBallsNum);
}

void ABreakPlayer::PlayCameraShake(float Scale)
{
	if (CameraShakeClass)
	{
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(CameraShakeClass, Scale);
	}
}

