// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/BreakHealthComponent.h"

// Sets default values for this component's properties
UBreakHealthComponent::UBreakHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBreakHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
}


// Called every frame
void UBreakHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBreakHealthComponent::DecreaseHealth()
{
	Health--;
	Health = FMath::Clamp(Health, 0, MaxHealth);
	if (Health <= 0) IsDeadDel.ExecuteIfBound(this->GetOwner());
}

