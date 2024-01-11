// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BreakPlayer.generated.h"

class ABreakBall;
class ABreakPaddle;
class ABreakBoxVolume;
class ABreakGameStateBase;
class ABreakPhysicsManager;
class UBreakHealthComponent;

DECLARE_MULTICAST_DELEGATE(FAllBallsLostDelegate)

UCLASS()
class BREAKOUTJR_API ABreakPlayer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABreakPlayer();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BreakOut | BreakPlayer | Properties")
		UBreakHealthComponent* PlayerHealthComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BreakOut | BreakPlayer | Properties", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<ABreakPaddle> PaddleBPClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BreakOut | BreakPlayer | Properties", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<ABreakBall> BallBPClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BreakOut | BreakPlayer | Properties", meta = (AllowPrivateAccess = "true"))
		int32 MaxBallsNum = 3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BreakOut | BreakPlayer | Properties", meta = (AllowPrivateAccess = "true"))
		float LaunchMagnitude = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BreakOut | BreakPlayer | VFX", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UMatineeCameraShake> CameraShakeClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BreakOut | BreakPlayer | VFX", meta = (AllowPrivateAccess = "true"))
		float OnBallLostCameraShakeScale = 0.08f;

	UPROPERTY(Transient)
		ABreakPhysicsManager* BreakPhysicsManagerRef = nullptr;

	UPROPERTY(Transient)
		ABreakPaddle* PaddleRef = nullptr;
	
	UPROPERTY(Transient)
		TArray<ABreakBoxVolume*> BoxVolumeBoundsArray;
	
	UPROPERTY(Transient)
		TArray<ABreakBall*> BallsArray;

	UPROPERTY(Transient)
		ABreakGameStateBase* BreakGameStateRef = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BreakOut | BreakPlayer | Properties")
		int32 BallsPlayingCount = 0;
	
	bool bIsBallOnPaddle = true;
	FVector BallSpawningLocation = FVector::ZeroVector;

	bool bAreAllBallsPlaying = false;

	int32 PaddleBouncesCounter = 0;

public:
	FAllBallsLostDelegate AllBallsLostDel;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void FindBoxVolumeBounds();
	void SpawnPaddle();
	void SpawnAllBalls();
	void AttachBallToPaddle(ABreakBall* BallToAttach);
	void DetachBallFromPaddle();

	UFUNCTION()
		void BallIsLost(AActor* BoxVolumeActor, AActor* BallActor);

	UFUNCTION()
		void PaddleBouncesEvents(AActor* PaddleActor, AActor* BallActor);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void MovePaddleRight(float AxisValue);
	void LaunchBall();

	void SetupBreakPlayer();

	void PlayCameraShake(float Scale);

	FORCEINLINE UBreakHealthComponent* GetHealthComponent() { return PlayerHealthComponent; }
	FORCEINLINE TArray<ABreakBall*> GetBallsArray() { return BallsArray; }
	
	void IncreaseBallsPlayingCount();
	FORCEINLINE int32 GetBallsPlayingCount() { return BallsPlayingCount; }
	FORCEINLINE int32 GetMaxBallsNum() { return MaxBallsNum; }

	FORCEINLINE void SetBreakGameStateRef(ABreakGameStateBase* Ref) { BreakGameStateRef = Ref; }
	FORCEINLINE void SetBreakPhysicsManagerRef(ABreakPhysicsManager* Ref) { BreakPhysicsManagerRef = Ref; }

	FORCEINLINE ABreakPaddle* GetBreakPaddleRef() { return PaddleRef; }
};
