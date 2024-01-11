// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Actors/BreakCircle.h"
#include "BreakCircle.h"

ABreakCircle::ABreakCircle()
{
	BodyType = EBodyType::EBT_CIRCLE;
}

void ABreakCircle::BeginPlay()
{
	Super::BeginPlay();

	if (BodyMesh)
	{
		CircleRadius = BodyMesh->GetRelativeScale3D().X * CircleRadius/2.0f;

		InitialRadius = CircleRadius;
		InitialScale = BodyMesh->GetRelativeScale3D();
	}
}

void ABreakCircle::SetRadius(float NewRadius)
{
	FVector NewScale = BodyMesh->GetRelativeScale3D() * NewRadius/90;
	BodyMesh->SetRelativeScale3D(NewScale);
	CircleRadius = BodyMesh->GetRelativeScale3D().X * NewRadius/2;
}

void ABreakCircle::ResetRadius()
{
	BodyMesh->SetRelativeScale3D(InitialScale);
	CircleRadius = InitialRadius;
}
