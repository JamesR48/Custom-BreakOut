// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BreakHealthComponent.generated.h"

DECLARE_DELEGATE_OneParam(IsDeadDelegate, AActor*)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BREAKOUTJR_API UBreakHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBreakHealthComponent();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BreakOut | Health | Properties", meta = (AllowPrivateAccess = "true"))
		int32 MaxHealth = 3;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BreakOut | Health | Properties")
		int32 Health = 0;
	bool bIsDead = false;

public:
	IsDeadDelegate IsDeadDel;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void DecreaseHealth();
	FORCEINLINE int32 GetCurrentHealth() { return Health; }
	FORCEINLINE void SetCurrentHealth(const int32 Val) { Health = Val; }
};
