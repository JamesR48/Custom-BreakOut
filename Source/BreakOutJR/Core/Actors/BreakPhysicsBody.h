// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BreakPhysicsBody.generated.h"

DECLARE_DELEGATE_TwoParams(IsCollidingDelegate, AActor*, AActor*)
//DECLARE_DELEGATE(IsCollidingDelegate)

UENUM(BlueprintType)
enum class EBodyType : uint8
{
	EBT_CIRCLE			UMETA(DeplayName = "Circle"),
	EBT_BOX				UMETA(DeplayName = "Box"),

	EBT_MAX				UMETA(DeplayName = "DefaulMAX")
};

UENUM(BlueprintType)
enum class ECollisionResponseType : uint8
{
	ECR_BLOCK			UMETA(DeplayName = "Block"),
	ECR_OVERLAP			UMETA(DeplayName = "Overlap"),
	ECR_IGNORE			UMETA(DeplayName = "Ignore"),

	ECR_MAX				UMETA(DeplayName = "DefaulMAX")
};

UENUM(BlueprintType)
enum class EPhysicsBehaviourType : uint8
{
	EPB_KINEMATIC		UMETA(DeplayName = "Kinematic"),
	EPB_STATIC			UMETA(DeplayName = "Static"),
	EPB_SIMULATED		UMETA(DeplayName = "Simulated"),

	ECR_MAX				UMETA(DeplayName = "DefaulMAX")
};

UCLASS()
class BREAKOUTJR_API ABreakPhysicsBody : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABreakPhysicsBody();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BreakOut | PhysBody | Mesh")
		UStaticMeshComponent* BodyMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BreakOut | PhysBody | Collision", meta = (AllowPrivateAccess = "true"))
		EBodyType BodyType = EBodyType::EBT_CIRCLE;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BreakOut | PhysBody | Collision", meta = (AllowPrivateAccess = "true"))
		ECollisionResponseType CollisionResponse = ECollisionResponseType::ECR_BLOCK;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BreakOut | PhysBody | Collision", meta = (AllowPrivateAccess = "true"))
		float CollisonRestitution = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BreakOut | PhysBody | Movement", meta = (AllowPrivateAccess = "true"))
		EPhysicsBehaviourType PhysicsBehaviour = EPhysicsBehaviourType::EPB_SIMULATED;		

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BreakOut | PhysBody | Movement", meta = (AllowPrivateAccess = "true"))
		float FrictionMagnitude = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BreakOut | PhysBody | Movement", meta = (AllowPrivateAccess = "true"))
		FVector MaxVelocity = FVector::ZeroVector;

	UPROPERTY(Transient)
		USceneComponent* BodyRoot = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BreakOut | PhysBody | Movement", meta = (AllowPrivateAccess = "true"))
		FVector Velocity = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BreakOut | PhysBody | Movement", meta = (AllowPrivateAccess = "true"))
		FVector Force = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BreakOut | PhysBody | Movement", meta = (AllowPrivateAccess = "true"))
		FVector Acceleration = FVector::ZeroVector;

	FVector InitialMaxVelocity = FVector::ZeroVector;

public:

	IsCollidingDelegate IsCollidingDel;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	virtual void Move(float ScaleValue);

	virtual void ApplyBodyImpulse(const FVector& Impulse);

	FORCEINLINE FVector GetBodyVelocity() { return Velocity; }
	FORCEINLINE void SetBodyVelocity(const FVector& Vel) { Velocity = Vel; }

	FORCEINLINE FVector GetBodyMaxVelocity() { return MaxVelocity; }
	FORCEINLINE void SetBodyMaxVelocity(const FVector& Vel) { MaxVelocity = Vel; }
	FORCEINLINE void ResetBodyMaxVelocity() { MaxVelocity = InitialMaxVelocity; }

	FORCEINLINE FVector GetBodyAcceleration() { return Acceleration; }
	FORCEINLINE void SetBodyAcceleration(const FVector& Acc) { Acceleration = Acc; }

	FORCEINLINE void AddBodyForce(const FVector& F) { Force += F; }
	FORCEINLINE void ClearBodyForces() { Force = FVector::ZeroVector; }

	FORCEINLINE bool GetIsBodyType(const EBodyType& Type) { return (BodyType == Type); }
	FORCEINLINE bool GetIsStatic() { return (PhysicsBehaviour == EPhysicsBehaviourType::EPB_STATIC); }
	FORCEINLINE bool GetIsKinematic() { return (PhysicsBehaviour == EPhysicsBehaviourType::EPB_KINEMATIC); }
	FORCEINLINE bool GetIsCollisionResponseType(const ECollisionResponseType& Type) { return (CollisionResponse == Type); }

	FORCEINLINE bool GetIsPhysicsBehaviourType(const EPhysicsBehaviourType& Type) { return (PhysicsBehaviour == Type); }

	FORCEINLINE float GetBodyFriction() { return FrictionMagnitude; }
	FORCEINLINE float GetRestitution() { return CollisonRestitution; }

	virtual void CallIsCollidingEvent(AActor* OtherActor);
	void SetIsBodyEnabled(bool bShouldEnable);
	bool GetIsBodyEnabled();
};
