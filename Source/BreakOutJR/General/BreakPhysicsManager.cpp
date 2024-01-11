// Fill out your copyright notice in the Description page of Project Settings.


#include "General/BreakPhysicsManager.h"
#include "BreakPhysicsManager.h"
#include "Kismet/GameplayStatics.h"

#include "Core/Actors/BreakPhysicsBody.h"
#include "Core/Actors/BreakCircle.h"
#include "Core/Actors/BreakBox.h"

// Sets default values
ABreakPhysicsManager::ABreakPhysicsManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABreakPhysicsManager::BeginPlay()
{
	Super::BeginPlay();
	
	FindAllPhysicsBodies();
}

// Called every frame
void ABreakPhysicsManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PhysicsBodiesCount == 0) return;

	TArray<FCollisionContactInfo> CollisionContactsArray;

	// Check for collisions
	for (int i = 0; i < PhysicsBodiesCount - 1; i++)
	{
		for (int j = i + 1; j < PhysicsBodiesCount; j++)
		{
			const bool bIsBodyACollidable = !PhysicsBodiesArray[i]->GetIsBodyEnabled() || PhysicsBodiesArray[i]->GetIsCollisionResponseType(ECollisionResponseType::ECR_IGNORE);
			const bool bIsBodyBCollidable = !PhysicsBodiesArray[j]->GetIsBodyEnabled() || PhysicsBodiesArray[j]->GetIsCollisionResponseType(ECollisionResponseType::ECR_IGNORE);

			if (bIsBodyACollidable || bIsBodyBCollidable) continue;

			if ( IsColliding(PhysicsBodiesArray[i], PhysicsBodiesArray[j], CollisionContactsArray) )
			{
				PhysicsBodiesArray[i]->CallIsCollidingEvent(PhysicsBodiesArray[j]);
				if (i >= PhysicsBodiesCount-1 || j >= PhysicsBodiesCount) continue;
				PhysicsBodiesArray[j]->CallIsCollidingEvent(PhysicsBodiesArray[i]);
			}
		}
	}

	// Resolve collisions
	if (CollisionContactsArray.Num() > 0)
	{
		for (FCollisionContactInfo Contacts : CollisionContactsArray)
		{
			SolveCollision(Contacts);
		}
	}

	// Apply movement
	for (ABreakPhysicsBody* Body : PhysicsBodiesArray)
	{
		if (!Body->GetIsBodyEnabled()) continue;

		Body->Move(DeltaTime);
	}
}

bool ABreakPhysicsManager::IsColliding(ABreakPhysicsBody* BodyA, ABreakPhysicsBody* BodyB, TArray<FCollisionContactInfo>& Contacts)
{
	if (!BodyA || !BodyB) return false;

	const bool IsCircleA = BodyA->GetIsBodyType(EBodyType::EBT_CIRCLE);
	const bool IsCircleB = BodyB->GetIsBodyType(EBodyType::EBT_CIRCLE);

	if (IsCircleA && IsCircleB)
	{
		return IsCircleCircleCollision(BodyA, BodyB, Contacts);
	}

	const bool IsBoxA = BodyA->GetIsBodyType(EBodyType::EBT_BOX);
	const bool IsBoxB = BodyB->GetIsBodyType(EBodyType::EBT_BOX);

	if ( IsCircleA && IsBoxB)
	{
		return IsCircleBoxCollision(BodyA, BodyB, Contacts);
	}
	if (IsBoxA && IsCircleB)
	{
		return IsCircleBoxCollision(BodyB, BodyA, Contacts);
	}

	return false;
}

bool ABreakPhysicsManager::IsCircleCircleCollision(ABreakPhysicsBody* BodyA, ABreakPhysicsBody* BodyB, TArray<FCollisionContactInfo>& Contacts)
{
	ABreakCircle* CircleA = Cast<ABreakCircle>(BodyA);
	ABreakCircle* CircleB = Cast<ABreakCircle>(BodyB);

	FVector CirclesDistance = BodyB->GetActorLocation() - BodyA->GetActorLocation();
	const float RadiusSum = CircleA->GetRadius() + CircleB->GetRadius();

	const bool bIsColliding = CirclesDistance.SizeSquared() <= (RadiusSum * RadiusSum);

	if (!bIsColliding) 
	{
		return false;
	}

	if (
		CircleA->GetIsCollisionResponseType(ECollisionResponseType::ECR_OVERLAP)
		|| CircleB->GetIsCollisionResponseType(ECollisionResponseType::ECR_OVERLAP)
		)
	{
		return true;
	}

	FCollisionContactInfo ContactInfo;
	ContactInfo.BodyA = BodyA;
	ContactInfo.BodyB = BodyB;
	
	CirclesDistance.Normalize();
	ContactInfo.CollisionNormal = CirclesDistance;

	ContactInfo.CollisionStartPoint = BodyB->GetActorLocation() - ContactInfo.CollisionNormal * CircleB->GetRadius();
	ContactInfo.CollisionEndPoint = BodyA->GetActorLocation() + ContactInfo.CollisionNormal * CircleA->GetRadius();

	ContactInfo.CollisionPenetrationDepth = (ContactInfo.CollisionEndPoint - ContactInfo.CollisionStartPoint).Size();

	Contacts.Add(ContactInfo);

	return true;
}

bool ABreakPhysicsManager::IsCircleBoxCollision(ABreakPhysicsBody* BodyA, ABreakPhysicsBody* BodyB, TArray<FCollisionContactInfo>& Contacts)
{
	ABreakCircle* CircleBody = Cast<ABreakCircle>(BodyA);
	ABreakBox* BoxBody = Cast<ABreakBox>(BodyB);

	FVector CircleCenter = CircleBody->GetActorLocation();
	FVector BoxCenter = BoxBody->GetActorLocation();
	
	/*
		 Find point on box closest to circle center for each coordinate axis,
		 if the point coordinate value is outside box, clamp it to the box,
		 else keep it as is
	*/
	float NearestX = FMath::Max(BoxCenter.X-BoxBody->GetWidth(), FMath::Min(CircleCenter.X, BoxCenter.X + BoxBody->GetWidth()));
	float NearestZ = FMath::Max(BoxCenter.Z-BoxBody->GetHeight(), FMath::Min(CircleCenter.Z, BoxCenter.Z + BoxBody->GetHeight()));

	// For getting a vector pointing FROM VecA TO VecB Do:
	// (Vector to point TO) - (Vector to point FROM) ----> VecB - VecA

	FVector BodiesDistance = CircleCenter - FVector(NearestX, 0.0f, NearestZ);

	/*
		Circle and box intersect if the (squared) distance from circle center 
		 to nearest point on box is less than the (squared) circle radius
	*/

	float CollisionPenetration = (CircleBody->GetRadius() * CircleBody->GetRadius()) - (BodiesDistance).SizeSquared();

	//const bool bIsColliding = (BodiesDistance.SizeSquared()) < (CircleBody->GetRadius() * CircleBody->GetRadius());
	const bool bIsColliding = CollisionPenetration >= 0.0f ;

	if (!bIsColliding)
	{
		return false;
	}

	if (
		CircleBody->GetIsCollisionResponseType(ECollisionResponseType::ECR_OVERLAP)
		|| BoxBody->GetIsCollisionResponseType(ECollisionResponseType::ECR_OVERLAP)
		)
	{
		return true;
	}

	FCollisionContactInfo ContactInfo;
	ContactInfo.BodyA = CircleBody;
	ContactInfo.BodyB = BoxBody;

	if (BoxBody->GetIsKinematic())
	{
		ContactInfo.bIsCollidingWithPaddle = true;
	}

	BodiesDistance.Normalize();
	ContactInfo.CollisionNormal = BodiesDistance;

	ContactInfo.CollisionPenetrationDepth = FMath::Sqrt(CollisionPenetration);

	Contacts.Add(ContactInfo);

	return true;
}

void ABreakPhysicsManager::SolvePenetration(const FCollisionContactInfo& ContactInfo)
{
	ABreakPhysicsBody* BodyA = ContactInfo.BodyA;
	ABreakPhysicsBody* BodyB = ContactInfo.BodyB;

	// If the bodies aren't static, move them apart in the direction of the collision normal so they don't intersect anymore
	if (BodyA->GetIsPhysicsBehaviourType(EPhysicsBehaviourType::EPB_SIMULATED) )
	{
		BodyA->AddActorLocalOffset(ContactInfo.CollisionNormal * ContactInfo.CollisionPenetrationDepth);
	}
	if (BodyB->GetIsPhysicsBehaviourType(EPhysicsBehaviourType::EPB_SIMULATED))
	{
		BodyB->AddActorLocalOffset(ContactInfo.CollisionNormal * ContactInfo.CollisionPenetrationDepth);
	}
}

void ABreakPhysicsManager::SolveCollision(const FCollisionContactInfo& ContactInfo)
{
	SolvePenetration(ContactInfo);

	if (ContactInfo.bIsCollidingWithPaddle)
	{
		BounceOnPaddle(ContactInfo.BodyA, ContactInfo.BodyB);
		return;
	}

	ABreakPhysicsBody* BodyA = ContactInfo.BodyA;
	ABreakPhysicsBody* BodyB = ContactInfo.BodyB;

	const float ElasticityCoefficient = FMath::Min(BodyA->GetRestitution(), BodyB->GetRestitution() );
	const FVector BodiesRelativeVelocity = BodyA->GetBodyVelocity() - BodyB->GetBodyVelocity();
	const float RelativeVelNormal = FVector::DotProduct(BodiesRelativeVelocity, ContactInfo.CollisionNormal);
	const float ImpulseMagnitude = ( -(1 + ElasticityCoefficient) * RelativeVelNormal)/2.0f;
	FVector Impulse = ContactInfo.CollisionNormal * ImpulseMagnitude;

	if (BodyA->GetIsPhysicsBehaviourType(EPhysicsBehaviourType::EPB_SIMULATED))
	{
		BodyA->ApplyBodyImpulse(Impulse);
	}
	if (BodyB->GetIsPhysicsBehaviourType(EPhysicsBehaviourType::EPB_SIMULATED))
	{
		BodyB->ApplyBodyImpulse(-Impulse);
	}
}

void ABreakPhysicsManager::BounceOnPaddle(ABreakPhysicsBody* BodyA, ABreakPhysicsBody* BodyB)
{
	ABreakBox* BoxBody = Cast<ABreakBox>(BodyB);
	float PaddleXHit = (BodyA->GetActorLocation().X - BoxBody->GetActorLocation().X) / (BoxBody->GetWidth() * 0.5f);
	float PaddleZHit = (BodyA->GetActorLocation().Z - BoxBody->GetActorLocation().Z) / (BoxBody->GetHeight() * 0.5f);

	FVector PaddleBounce = FVector(PaddleXHit, 0.0f, PaddleZHit);
	//PaddleBounce.Normalize();
	BodyA->ApplyBodyImpulse(PaddleBounce * BodyA->GetBodyVelocity().Size());
}

FVector ABreakPhysicsManager::GenerateFrictionForce(ABreakPhysicsBody* Body, float Magnitude)
{
	FVector BodyVelocity = Body->GetBodyVelocity();
	BodyVelocity.Normalize();
	// Calculate the friction force based on a magnitude and the inverse direction of the bodies velocity
	FVector FrictionForce = -BodyVelocity * Magnitude;
	return FrictionForce;
}

void ABreakPhysicsManager::FindAllPhysicsBodies()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABreakPhysicsBody::StaticClass(), FoundActors);
	PhysicsBodiesCount = FoundActors.Num();
	
	if (PhysicsBodiesCount == 0) return;

	for (AActor* Actor : FoundActors)
	{
		ABreakPhysicsBody* Body = Cast<ABreakPhysicsBody>(Actor);
		if (!Body) continue;

		PhysicsBodiesArray.Add(Body);
	}
}

void ABreakPhysicsManager::AddPhysicsBody(AActor* Body)
{
	if (!Body) return;
	PhysicsBodiesArray.Add( (ABreakPhysicsBody*)Body );
	PhysicsBodiesCount += 1;
}

void ABreakPhysicsManager::RemovePhysicsBody(AActor* Body)
{
	if (!Body) return;
	PhysicsBodiesArray.RemoveSingleSwap((ABreakPhysicsBody*)Body);
	PhysicsBodiesCount -= 1;
}
