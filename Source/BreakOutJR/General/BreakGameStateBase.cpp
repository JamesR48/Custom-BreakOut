// Fill out your copyright notice in the Description page of Project Settings.


#include "General/BreakGameStateBase.h"
#include "Kismet/GameplayStatics.h"

#include "General/BreakPhysicsManager.h"
#include "General/BreakBricksManager.h"
#include "General/BreakPowerUpsManager.h"
#include "Player/BreakPlayer.h"
#include "Components/BreakHealthComponent.h"
#include "General/BreakPlayerController.h"


ABreakGameStateBase::ABreakGameStateBase()
{

}

void ABreakGameStateBase::BeginPlay()
{
	Super::BeginPlay();

	FindBreakPhysicsManager();
	FindBreakPlayer();
	FindBreakPlayerController();
	FindBreakBricksManager();
	FindBreakPowerUpsManager();
}

void ABreakGameStateBase::FindBreakPhysicsManager()
{
	AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ABreakPhysicsManager::StaticClass());
	if (!FoundActor) return;

	BreakPhysicsManagerRef = Cast<ABreakPhysicsManager>(FoundActor);
}

void ABreakGameStateBase::FindBreakBricksManager()
{
	AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ABreakBricksManager::StaticClass());
	if (!FoundActor) return;

	BreakBricksManagerRef = Cast<ABreakBricksManager>(FoundActor);
	if (!BreakBricksManagerRef || !BreakPhysicsManagerRef) return;

	BreakBricksManagerRef->SetBreakGameStateRef(this);
	BreakBricksManagerRef->SetBreakPhysicsManagerRef(BreakPhysicsManagerRef);
	BreakBricksManagerRef->SetupBreakBricksManager();

	AllBricksInLevelNum = BreakBricksManagerRef->GetMaxBricksNum();
}

void ABreakGameStateBase::FindBreakPowerUpsManager()
{
	AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ABreakPowerUpsManager::StaticClass());
	if (!FoundActor) return;

	BreakPowerUpsManagerRef = Cast<ABreakPowerUpsManager>(FoundActor);
	if (!BreakPowerUpsManagerRef || !BreakPhysicsManagerRef) return;

	BreakPowerUpsManagerRef->SetBreakPhysicsManagerRef(BreakPhysicsManagerRef);

	if (!BreakBricksManagerRef) return;
	BreakPowerUpsManagerRef->SetMaxBricksCount(AllBricksInLevelNum);
	BreakPowerUpsManagerRef->SetupBreakPowerUpsManager();
}

void ABreakGameStateBase::FindBreakPlayer()
{
	AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ABreakPlayer::StaticClass());
	if (!FoundActor) return;

	BreakPlayerRef = Cast<ABreakPlayer>(FoundActor);
	if (!BreakPlayerRef  || !BreakPhysicsManagerRef) return;

	BreakPlayerRef->SetBreakPhysicsManagerRef(BreakPhysicsManagerRef);
	BreakPlayerRef->SetBreakGameStateRef(this);
	BreakPlayerRef->SetupBreakPlayer();

	UBreakHealthComponent* PlayerHealthComponent = BreakPlayerRef->GetHealthComponent();
	if (!PlayerHealthComponent) return;

	PlayerHealthComponent->IsDeadDel.BindUObject(this, &ABreakGameStateBase::CheckIsPlayerDead);
}

void ABreakGameStateBase::CheckIsPlayerDead(AActor* DeadActor)
{
	ABreakPlayer* DeadPlayer = Cast<ABreakPlayer>(DeadActor);
	if (!DeadPlayer) return;

	//PLAYER LOST
	bIsGameOver = true;
	IsGameOverDel.ExecuteIfBound(false); //boolean bHasPlayerWon
	UE_LOG(LogTemp, Error, TEXT("PLAYER LOST"));
	
	// Restart level after delay
	GetWorldTimerManager().SetTimer(RestartLevelTimerHandle, this, &ABreakGameStateBase::RestartLevel, RestarLevelTime, false);
}

void ABreakGameStateBase::BrickDestroyedEvent(const FVector& BrickPosition, const int32 BrickScoreValue)
{
	if (!BreakPowerUpsManagerRef || !BreakPlayerRef || !BreakBricksManagerRef || bIsGameOver) return;

	BreakPowerUpsManagerRef->SpawnPowerUpAtPosition(BrickPosition);
	
	PlayerScore += BrickScoreValue;
	if (BreakPlayerControllerRef) BreakPlayerControllerRef->UpdatePlayerScoreWidget(PlayerScore);
	
	DestroyedBricksCount++;

	if (DestroyedBricksCount >= AllBricksInLevelNum)
	{
		//PLAYER WON
		bIsGameOver = true;
		IsGameOverDel.ExecuteIfBound(true); //boolean bHasPlayerWon
		UE_LOG(LogTemp, Error, TEXT("PLAYER WON"));
		
		// Restart level after delay
		GetWorldTimerManager().SetTimer(RestartLevelTimerHandle, this, &ABreakGameStateBase::RestartLevel, RestarLevelTime, false);
	}
}

void ABreakGameStateBase::FindBreakPlayerController()
{
	APlayerController* FoundController = GetWorld()->GetFirstPlayerController();
	if (FoundController) BreakPlayerControllerRef = Cast<ABreakPlayerController>(FoundController);

	if (BreakPlayerControllerRef && BreakPlayerRef)
	{
		BreakPlayerControllerRef->SetBreakGameStateRef(this);
		BreakPlayerControllerRef->SetBreakPlayerRef(BreakPlayerRef);
	}
}

void ABreakGameStateBase::RestartLevel()
{
	UWorld* WorldRef = GetWorld();
	if (WorldRef) UGameplayStatics::OpenLevel(WorldRef, FName(WorldRef->GetName()), false);
}
