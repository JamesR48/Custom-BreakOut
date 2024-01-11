// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Actors/BreakPhysicsBody.h"
#include "BreakBox.generated.h"

/**
 * 
 */
UCLASS()
class BREAKOUTJR_API ABreakBox : public ABreakPhysicsBody
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ABreakBox();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BreakOut | PhysBox | Properties", meta = (AllowPrivateAccess = "true"))
		float BoxWidth = 100.0f; // 100cm

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BreakOut | PhysBox | Properties", meta = (AllowPrivateAccess = "true"))
		float BoxHeight = 100.0f; // 100cm

	float InitialWidth = 0.0f;
	FVector InitialScale = FVector::ZeroVector;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void SetWidth(float NewWidth);
	void ResetWidth();

	FORCEINLINE float GetWidth() { return BoxWidth; }
	FORCEINLINE float GetHeight() { return BoxHeight; }
};
