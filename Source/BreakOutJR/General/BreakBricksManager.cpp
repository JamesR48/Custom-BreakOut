// Fill out your copyright notice in the Description page of Project Settings.


#include "General/BreakBricksManager.h"
#include "Kismet/GameplayStatics.h"

#include "Actors/BreakBrick.h"
#include "Components/BreakHealthComponent.h"
#include "General/BreakPhysicsManager.h"
#include "General/BreakGameStateBase.h"

// Sets default values
ABreakBricksManager::ABreakBricksManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABreakBricksManager::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABreakBricksManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABreakBricksManager::SetupBreakBricksManager()
{
	SpawnAllBricks();
}

void ABreakBricksManager::SpawnAllBricks()
{
	if (!BrickBPClass || !BreakPhysicsManagerRef) return;

	const int32 MaxBricksNum = GetMaxBricksNum();

	int32 BricksCounter = 0;
	int32 BrickHealth = 1;
	int32 BrickColorIndex = 0; 

	for (int32 idx = 0; idx < MaxBricksNum; idx++)
	{
		ABreakBrick* Brick = GetWorld()->SpawnActor<ABreakBrick>(BrickBPClass, GetActorLocation(), FRotator::ZeroRotator);
		if (!Brick) continue;

		const FVector SpawnLocation = FVector((idx % MaxBricksPerRow)*Brick->GetWidth()*BricksSpacing + GetActorLocation().X, 0.0f, (idx / MaxBricksPerRow)*Brick->GetHeight()*BricksSpacing + GetActorLocation().Z);
		Brick->SetActorLocation(SpawnLocation);
		BreakPhysicsManagerRef->AddPhysicsBody((AActor*)Brick);
		BricksArray.Add(Brick);

		BricksCounter++;

		Brick->IsCollidingDel.BindUObject(this, &ABreakBricksManager::OnBrickCollided);
		
		Brick->SetBrickColor(BrickColorsArray[BrickColorIndex]);

		UBreakHealthComponent* BrickHealthComp = Brick->GetHealthComponent();
		if (!BrickHealthComp) return;

		BrickHealthComp->IsDeadDel.BindUObject(this, &ABreakBricksManager::OnBrickDestroyed);
		BrickHealthComp->SetCurrentHealth(BrickHealth);
		
		Brick->SetBrickScoreValue(BrickHealth * 100);

		if (BricksCounter >= MaxBricksPerRow*2)
		{
			BrickHealth++;
			BrickColorIndex++;
			BricksCounter = 0;
		}
	}
}

void ABreakBricksManager::OnBrickCollided(AActor* ThisBrick, AActor* OtherActor)
{
	if (!ThisBrick) return;

	ABreakBrick* BrickPhysics = Cast<ABreakBrick>(ThisBrick);
	if (!BrickPhysics) return;

	UBreakHealthComponent* BrickHealthComp = BrickPhysics->GetHealthComponent();	
	if (!BrickHealthComp) return;
	
	BrickHealthComp->DecreaseHealth();
}

void ABreakBricksManager::OnBrickDestroyed(AActor* ThisBrick)
{
	ABreakBrick* BrickPhysics = Cast<ABreakBrick>(ThisBrick);
	if (!BrickPhysics || !BreakPhysicsManagerRef) return;

	BrickPhysics->IsCollidingDel.Unbind();
	BrickPhysics->GetHealthComponent()->IsDeadDel.Unbind();
	BricksArray.RemoveSingleSwap(BrickPhysics);
	
	BreakPhysicsManagerRef->RemovePhysicsBody(BrickPhysics);

	if (!BreakGameStateRef) return;
	BreakGameStateRef->BrickDestroyedEvent(BrickPhysics->GetActorLocation(), BrickPhysics->GetBrickScoreValue());

	BrickPhysics->Destroy();
}
