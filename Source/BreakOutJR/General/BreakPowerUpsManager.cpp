// Fill out your copyright notice in the Description page of Project Settings.


#include "General/BreakPowerUpsManager.h"
#include "Kismet/GameplayStatics.h"

#include "Core/Actors/BreakPowerUp.h"
#include "General/BreakPhysicsManager.h"

// Sets default values
ABreakPowerUpsManager::ABreakPowerUpsManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABreakPowerUpsManager::BeginPlay()
{
	Super::BeginPlay();
	}

// Called every frame
void ABreakPowerUpsManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABreakPowerUpsManager::SetupBreakPowerUpsManager()
{
	SpawnAllPowerUps();
}

void ABreakPowerUpsManager::SpawnAllPowerUps()
{
	if (PowerUpBPClassesArray.Num() == 0 || !BreakPhysicsManagerRef || MaxBricksCount == 0) return;

	MaxPowerUpsInGame = FMath::Clamp(MaxPowerUpsInGame, 1, MaxBricksCount);

	for (int32 idx = 0; idx < MaxPowerUpsInGame; idx++)
	{
		const int32 RandPowerUpBPClass = FMath::RandRange(0, PowerUpBPClassesArray.Num() - 1);
		TSubclassOf<ABreakPowerUp> PowerUpBPClassRef = PowerUpBPClassesArray[RandPowerUpBPClass];
		ABreakPowerUp* PowerUp = GetWorld()->SpawnActor<ABreakPowerUp>(PowerUpBPClassRef, GetActorLocation(), FRotator::ZeroRotator);
		if (!PowerUp) continue;

		BreakPhysicsManagerRef->AddPhysicsBody((AActor*)PowerUp);
		PowerUpsArray.Add(PowerUp);

		PowerUp->IsCollidingDel.BindUObject(this, &ABreakPowerUpsManager::OnPowerUpCollided);
	}
}

void ABreakPowerUpsManager::SpawnPowerUpAtPosition(FVector Position)
{
	if (PowerUpsArray.Num() == 0 || CurrentPowerUpsInGame == MaxPowerUpsInGame) return;

	int32 RandPowerUpIndex = FMath::RandRange(0, MaxPowerUpsInGame - 1);
	
	if ( PowerUpsArray[RandPowerUpIndex]->GetIsPowerUpActive() ) return;

	PowerUpsArray[RandPowerUpIndex]->SetActorLocation(Position);
	PowerUpsArray[RandPowerUpIndex]->SetIsBodyEnabled(true);

	CurrentPowerUpsInGame++;
}

void ABreakPowerUpsManager::OnPowerUpCollided(AActor* ThisPowerUp, AActor* OtherActor)
{
	if (!ThisPowerUp || !OtherActor) return;

	ABreakPowerUp* PowerUpPhysics = Cast<ABreakPowerUp>(ThisPowerUp);
	if (!PowerUpPhysics) return;

	PowerUpPhysics->ActivatePowerUp(OtherActor);
	PowerUpPhysics->SetIsBodyEnabled(false);

	CurrentPowerUpsInGame--;
}

