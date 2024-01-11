// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Actors/BreakCircle.h"
#include "BreakBall.generated.h"

class UParticleSystem;
class USoundCue;

class ABreakPowerUp;

UCLASS()
class BREAKOUTJR_API ABreakBall : public ABreakCircle
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ABreakBall();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BreakOut | Ball | VFX", meta = (AllowPrivateAccess = "true"))
		UParticleSystem* CollisionParticles;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BreakOut | Ball | SFX", meta = (AllowPrivateAccess = "true"))
		USoundCue* CollisionSoundCue;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BreakOut | Ball | VFX")
		UParticleSystemComponent* ParticleTrail;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BreakOut | Ball | VFX", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UMatineeCameraShake> CameraShakeClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BreakOut | Ball | VFX", meta = (AllowPrivateAccess = "true"))
		float CollisionCameraShakeScale = 0.01f;

	UPROPERTY(Transient)
		ABreakPowerUp* CurrentActivePowerUp = nullptr;

	int32 PaddleBouncesCount = 0;
	
	bool bIsLost = false;

protected:
	void PlayCameraShake(float Scale);

public:
	void SetActivePowerUp(ABreakPowerUp* NewPowerUp);
	void DeactivatePowerUp();

	virtual void CallIsCollidingEvent(AActor* OtherActor) override;

	FORCEINLINE bool GetBallIsLost() { return bIsLost; }
	FORCEINLINE void SetBallIsLost(bool Val) { bIsLost = Val; }
	FORCEINLINE ABreakPowerUp* GetActivePowerUp() { return CurrentActivePowerUp; }

	FORCEINLINE int32 GetPaddleBouncesCount() { return PaddleBouncesCount; }
	FORCEINLINE void SetPaddleBouncesCount(int32 NewVal) { PaddleBouncesCount = NewVal; }
	FORCEINLINE void UpdatePaddleBouncesCount() { PaddleBouncesCount++; }
};
