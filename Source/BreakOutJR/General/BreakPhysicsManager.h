// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BreakPhysicsManager.generated.h"

class ABreakPhysicsBody;
class ABreakCircle;
class ABreakBox;

USTRUCT(BlueprintType)
struct FCollisionContactInfo
{
	GENERATED_BODY()

	UPROPERTY(Transient)
		ABreakPhysicsBody* BodyA;
	UPROPERTY(Transient)
		ABreakPhysicsBody* BodyB = nullptr;

	FVector CollisionStartPoint = FVector::ZeroVector;
	FVector CollisionEndPoint = FVector::ZeroVector;
	FVector CollisionNormal = FVector::ZeroVector;
	float CollisionPenetrationDepth = 0.0f;
	bool bIsCollidingWithPaddle = false;
};

UCLASS()
class BREAKOUTJR_API ABreakPhysicsManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABreakPhysicsManager();

protected:
	UPROPERTY(Transient)
		TArray<ABreakPhysicsBody*> PhysicsBodiesArray;

	int32 PhysicsBodiesCount = 0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool IsColliding(ABreakPhysicsBody* BodyA, ABreakPhysicsBody* BodyB, TArray<FCollisionContactInfo>& Contacts);
	bool IsCircleCircleCollision(ABreakPhysicsBody* BodyA, ABreakPhysicsBody* BodyB, TArray<FCollisionContactInfo>& Contacts);
	bool IsCircleBoxCollision(ABreakPhysicsBody* BodyA, ABreakPhysicsBody* BodyB, TArray<FCollisionContactInfo>& Contacts);
	
	void SolvePenetration(const FCollisionContactInfo& ContactInfo);
	void SolveCollision(const FCollisionContactInfo& ContactInfo);

	void FindAllPhysicsBodies();

	void BounceOnPaddle(ABreakPhysicsBody* BodyA, ABreakPhysicsBody* BodyB);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AddPhysicsBody(AActor* Body);
	void RemovePhysicsBody(AActor* Body);
	FVector GenerateFrictionForce(ABreakPhysicsBody* Body, float Magnitude);
};
