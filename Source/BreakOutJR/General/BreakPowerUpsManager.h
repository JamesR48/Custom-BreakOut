// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BreakPowerUpsManager.generated.h"

class ABreakPowerUp;
class ABreakPhysicsManager;
class ABreakGameStateBase;

UCLASS()
class BREAKOUTJR_API ABreakPowerUpsManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABreakPowerUpsManager();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BreakOut | PowerUpsManager | Properties", meta = (AllowPrivateAccess = "true"))
		TArray < TSubclassOf<ABreakPowerUp> > PowerUpBPClassesArray;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BreakOut | PowerUpsManager | Properties", meta = (AllowPrivateAccess = "true"))
		int32 MaxPowerUpsInGame = 10;

	UPROPERTY(Transient)
		TArray<ABreakPowerUp*> PowerUpsArray;

	UPROPERTY(Transient)
		ABreakPhysicsManager* BreakPhysicsManagerRef = nullptr;

	int32 CurrentPowerUpsInGame = 0;
	int32 MaxBricksCount = 0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnAllPowerUps();

	UFUNCTION()
		void OnPowerUpCollided(AActor* ThisPowerUp, AActor* OtherActor);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetupBreakPowerUpsManager();

	void SpawnPowerUpAtPosition(FVector Position);

	FORCEINLINE void SetBreakPhysicsManagerRef(ABreakPhysicsManager* Ref) { BreakPhysicsManagerRef = Ref; }
	FORCEINLINE void SetMaxBricksCount(const int32 Count) { MaxBricksCount = Count; }
};
