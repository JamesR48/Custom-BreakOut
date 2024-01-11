// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BreakGameStateBase.generated.h"

class ABreakPlayer;
class ABreakBricksManager;
class ABreakPowerUpsManager;
class ABreakPhysicsManager;
class ABreakPlayerController;

DECLARE_DELEGATE_OneParam(IsGameOverDelegate, bool)

UCLASS()
class BREAKOUTJR_API ABreakGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	ABreakGameStateBase();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BreakOut | GameInfo | Properties", meta = (AllowPrivateAccess = "true"))
		float RestarLevelTime = 4.0f;

	UPROPERTY(Transient)
		ABreakPlayer* BreakPlayerRef = nullptr;

	UPROPERTY(Transient)
		ABreakPowerUpsManager* BreakPowerUpsManagerRef = nullptr;

	UPROPERTY(Transient)
		ABreakBricksManager* BreakBricksManagerRef = nullptr;

	UPROPERTY(Transient)
		ABreakPhysicsManager* BreakPhysicsManagerRef = nullptr;

	UPROPERTY(Transient)
		ABreakPlayerController* BreakPlayerControllerRef = nullptr;

	int32 PlayerScore = 0;
	int32 DestroyedBricksCount = 0;
	int32 AllBricksInLevelNum = 0;

	FTimerHandle RestartLevelTimerHandle;

	bool bIsGameOver = false;

public:
	IsGameOverDelegate IsGameOverDel;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void FindBreakPhysicsManager();
	void FindBreakBricksManager();
	void FindBreakPowerUpsManager();
	void FindBreakPlayer();
	void FindBreakPlayerController();

	UFUNCTION()
		void CheckIsPlayerDead(AActor* DeadActor);

	UFUNCTION()
		void RestartLevel();

public:
	void BrickDestroyedEvent(const FVector& BrickPosition, const int32 BrickScoreValue);

	FORCEINLINE bool GetIsGameOver() { return bIsGameOver; }
};
