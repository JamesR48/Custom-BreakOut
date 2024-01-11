// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Actors/BreakPhysicsBody.h"
#include "BreakCircle.generated.h"

/**
 * 
 */
UCLASS()
class BREAKOUTJR_API ABreakCircle : public ABreakPhysicsBody
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ABreakCircle();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BreakOut | PhysCircle | Properties", meta = (AllowPrivateAccess = "true"))
		float CircleRadius = 100.0f; // 100cm

	float InitialRadius = 0.0f;
	FVector InitialScale = FVector::ZeroVector;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	FORCEINLINE float GetRadius() { return CircleRadius; }
	void SetRadius(float NewRadius);
	void ResetRadius();
};
