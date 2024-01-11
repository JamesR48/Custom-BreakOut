// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BreakBricksManager.generated.h"

class ABreakBrick;
class ABreakPowerUp;
class ABreakPhysicsManager;
class ABreakGameStateBase;

UCLASS()
class BREAKOUTJR_API ABreakBricksManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABreakBricksManager();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BreakOut | BricksManager | Properties", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<ABreakBrick> BrickBPClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BreakOut | BricksManager | Properties", meta = (AllowPrivateAccess = "true"))
		int32 MaxBricksPerRow = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BreakOut | BricksManager | Properties", meta = (AllowPrivateAccess = "true"))
		int32 MaxBricksPerColumn = 6;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BreakOut | BricksManager | Properties", meta = (AllowPrivateAccess = "true"))
		float BricksSpacing = 2.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BreakOut | BricksManager | Properties", meta = (AllowPrivateAccess = "true"))
		TArray<FColor> BrickColorsArray;

	UPROPERTY(Transient)
		ABreakPhysicsManager* BreakPhysicsManagerRef = nullptr;

	UPROPERTY(Transient)
		TArray<ABreakBrick*> BricksArray;
	
	UPROPERTY(Transient)
		ABreakGameStateBase* BreakGameStateRef = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void SpawnAllBricks();

	UFUNCTION()
		void OnBrickCollided(AActor* ThisBrick, AActor* OtherActor);
	UFUNCTION()
		void OnBrickDestroyed(AActor* ThisBrick);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetupBreakBricksManager();

	FORCEINLINE void SetBreakGameStateRef(ABreakGameStateBase* Ref) { BreakGameStateRef = Ref; }
	FORCEINLINE void SetBreakPhysicsManagerRef(ABreakPhysicsManager* Ref) { BreakPhysicsManagerRef = Ref; }
	FORCEINLINE const int32 GetMaxBricksNum() { return (MaxBricksPerColumn * MaxBricksPerRow); }
};
