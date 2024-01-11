// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Actors/BreakBox.h"
#include "BreakBrick.generated.h"

class UBreakHealthComponent;

UCLASS()
class BREAKOUTJR_API ABreakBrick : public ABreakBox
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABreakBrick();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BreakOut | BreakBrick | Properties", meta = (AllowPrivateAccess = "true"))
		int32 BrickScoreValue = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BreakOut | BreakBrick | Properties")
		UBreakHealthComponent* BrickHealthComponent = nullptr;

	UPROPERTY(Transient)
		UMaterialInstanceDynamic* BrickDynamicMaterial = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	FORCEINLINE UBreakHealthComponent* GetHealthComponent() { return BrickHealthComponent; }
	FORCEINLINE void SetBrickColor(const FColor& NewColor) {if(BrickDynamicMaterial) BrickDynamicMaterial->SetVectorParameterValue(TEXT("Color"), NewColor); }
	
	FORCEINLINE void SetBrickScoreValue(const int32 Val) { BrickScoreValue = Val; }
	FORCEINLINE int32 GetBrickScoreValue() { return BrickScoreValue; }
};
