// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/BreakBrick.h"

#include "Components/BreakHealthComponent.h"

ABreakBrick::ABreakBrick()
{
	BrickHealthComponent = CreateDefaultSubobject<UBreakHealthComponent>(TEXT("BrickHealthComponent"));
}

void ABreakBrick::BeginPlay()
{
	Super::BeginPlay();

	BrickDynamicMaterial = BodyMesh->CreateDynamicMaterialInstance(0);
}
