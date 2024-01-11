// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Actors/BreakBox.h"
#include "BreakPaddle.generated.h"

class ABreakPowerUp;

UCLASS()
class BREAKOUTJR_API ABreakPaddle : public ABreakBox
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BreakOut | Paddle | Properties", meta = (AllowPrivateAccess = "true"))
		float PaddleMoveSpeed = 2000.0f; //2000cm

	//UPROPERTY(Transient)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BreakOut | Paddle | Properties")
		ABreakPowerUp* CurrentActivePowerUp = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BreakOut | Paddle | Properties")
		int32 PaddleBouncesCount = 0;

public:
	void MoveRight(float AxisValue);

	void SetActivePowerUp(ABreakPowerUp* NewPowerUp);
	void DeactivatePowerUp();

	FORCEINLINE ABreakPowerUp* GetActivePowerUp() { return CurrentActivePowerUp; }

	FORCEINLINE int32 GetPaddleBouncesCount() { return PaddleBouncesCount; }
	FORCEINLINE void SetPaddleBouncesCount(int32 NewVal) { PaddleBouncesCount = NewVal; }
	FORCEINLINE void UpdatePaddleBouncesCount() { PaddleBouncesCount++; }
};
