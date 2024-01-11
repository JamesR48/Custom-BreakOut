// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Actors/BreakBox.h"
#include "BreakBox.h"

ABreakBox::ABreakBox()
{
	BodyType = EBodyType::EBT_BOX;
}

void ABreakBox::BeginPlay()
{
	Super::BeginPlay();

	if (BodyMesh)
	{
		BoxWidth = BodyMesh->GetRelativeScale3D().X * BoxWidth/2;
		BoxHeight = BodyMesh->GetRelativeScale3D().Z * BoxHeight/2;

		InitialWidth = BoxWidth;
		InitialScale = BodyMesh->GetRelativeScale3D();
	}
}

void ABreakBox::SetWidth(float NewWidth)
{
	FVector NewScale = FVector(BodyMesh->GetRelativeScale3D().X * NewWidth / 90, BodyMesh->GetRelativeScale3D().Y, BodyMesh->GetRelativeScale3D().Z);
	BodyMesh->SetRelativeScale3D(NewScale);
	BoxWidth = BodyMesh->GetRelativeScale3D().X * NewWidth / 2;
}

void ABreakBox::ResetWidth()
{
	BodyMesh->SetRelativeScale3D(InitialScale);
	BoxWidth = InitialWidth;
}
