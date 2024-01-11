// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Actors/BreakPhysicsBody.h"
#include "BreakPhysicsBody.h"

// Sets default values
ABreakPhysicsBody::ABreakPhysicsBody()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BodyRoot = CreateDefaultSubobject<USceneComponent>("BodyRoot");
	RootComponent = BodyRoot;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>("BodyMesh");
	BodyMesh->SetupAttachment(BodyRoot);
}

void ABreakPhysicsBody::BeginPlay()
{
	Super::BeginPlay();

	InitialMaxVelocity = MaxVelocity;
}

void ABreakPhysicsBody::Move(float ScaleValue)
{
	if (GetIsStatic()) return;

	// Find Acceleration based on the forces applied (Mass is neglected since it'll always be 1)
	Acceleration += Force;

	// Integrate the Acceleration to find the new Velocity (NewVel = Vel + Accel * Time) and clamp it
	Velocity += Acceleration * ScaleValue;
	Velocity = ClampVector(Velocity, -MaxVelocity, MaxVelocity);

	// Integrate the Velocity to find the new Position (NewPos = Pos + Vel * Time)
	const FVector NewLocation = GetActorLocation() + Velocity * FMath::Abs(ScaleValue);
	SetActorLocation(NewLocation);

	ClearBodyForces();
}

void ABreakPhysicsBody::ApplyBodyImpulse(const FVector& Impulse)
{
	if (GetIsStatic() || GetIsKinematic()) return;

	Velocity += Impulse;
}

void ABreakPhysicsBody::CallIsCollidingEvent(AActor* OtherActor)
{
	IsCollidingDel.ExecuteIfBound(this, OtherActor);
}

void ABreakPhysicsBody::SetIsBodyEnabled(bool bShouldEnable)
{
	if (bShouldEnable)
	{
		SetActorHiddenInGame(false);
		SetHidden(false);
		SetActorTickEnabled(true);
	}
	else
	{
		SetActorHiddenInGame(true);
		SetHidden(true);
		SetActorTickEnabled(false);
		SetBodyVelocity(FVector::ZeroVector);
	}
}

bool ABreakPhysicsBody::GetIsBodyEnabled()
{
	bool bIsEnabled = !this->IsHidden() && this->IsActorTickEnabled();
	return bIsEnabled;
}

