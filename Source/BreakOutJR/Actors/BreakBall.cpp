// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/BreakBall.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ABreakBall::ABreakBall()
{
	ParticleTrail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BallDustTrail"));
	ParticleTrail->SetupAttachment(RootComponent);
}

void ABreakBall::SetActivePowerUp(ABreakPowerUp* NewPowerUp)
{
	if (CurrentActivePowerUp && CurrentActivePowerUp == NewPowerUp) return;
	
	CurrentActivePowerUp = NewPowerUp;
}

void ABreakBall::DeactivatePowerUp()
{
	if (!CurrentActivePowerUp) return;
	CurrentActivePowerUp = nullptr;
	ResetRadius(); 
	ResetBodyMaxVelocity();
	PaddleBouncesCount = 0;
}

void ABreakBall::CallIsCollidingEvent(AActor* OtherActor)
{
	Super::CallIsCollidingEvent(OtherActor);

	if (CollisionParticles) UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CollisionParticles, GetActorLocation(), FRotator::ZeroRotator, FVector::OneVector*0.3f, true);

	if (CollisionSoundCue) UGameplayStatics::PlaySound2D(GetWorld(), CollisionSoundCue);

	PlayCameraShake(CollisionCameraShakeScale);
}

void ABreakBall::PlayCameraShake(float Scale)
{
	if (CameraShakeClass)
	{
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(CameraShakeClass, Scale);
	}
}