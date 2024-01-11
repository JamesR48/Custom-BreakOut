// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BreakPlayerController.generated.h"

class UUserWidget;
class UTimelineComponent;
class UCurveFloat;

class ABreakPlayer;
class ABreakGameStateBase;
class UBreakHealthComponent;
class UBreakTextDisplayerWidget;

UCLASS()
class BREAKOUTJR_API ABreakPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	ABreakPlayerController();

protected:
	/** Reference to the UMG asset in the editor */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BreakOut | BreakPlayerController | Widgets", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UUserWidget> HUDOverlayAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BreakOut | BreakPlayerController | Widgets", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UUserWidget> GameOverScreenAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BreakOut | BreakPlayerController | Widgets")
		UTimelineComponent* GameOverScreenOpacityTimeline;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BreakOut | BreakPlayerController | Widgets", meta = (AllowPrivateAccess = "true"))
		UCurveFloat* GameOverScreenOpacityFloatCurve;

	/** Variable to hold the widget after creating it */
	UPROPERTY(Transient)
		UUserWidget* HUDOverlayRef = nullptr;

	UPROPERTY(Transient)
		UBreakTextDisplayerWidget* GameOverScreenRef = nullptr;

	UPROPERTY(Transient)
		UBreakTextDisplayerWidget* PlayerScoreTextRef = nullptr;

	UPROPERTY(Transient)
		UBreakTextDisplayerWidget* PlayerLivesTextRef = nullptr;

	UPROPERTY(Transient)
		ABreakPlayer* BreakPlayerRef = nullptr;

	UPROPERTY(Transient)
		ABreakGameStateBase* BreakGameStateRef = nullptr;
	
	UPROPERTY(Transient)
		UBreakHealthComponent* PlayerHealthCompRef = nullptr;;

	bool bCanReceiveInput = true;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void FindLevelCamera();
	void CreateWidgets();

	UFUNCTION()
		void MovePaddleRight(float AxisValue);

	UFUNCTION()
		void LaunchBall();

	UFUNCTION()
		void StartGameOverEvents(bool bHasPlayerWon);

	UFUNCTION()
		void UpdateGameOverScreenOpacity(float value);

	UFUNCTION()
		void UpdatePlayerLivesWidget();

	void DisplayGameOverScreen(bool bHasPlayerWon);

public:
	// Called to bind functionality to input
	virtual void SetupInputComponent() override;

	void UpdatePlayerScoreWidget(const int32 NewScore);

	void SetBreakGameStateRef(ABreakGameStateBase* Ref);
	void SetBreakPlayerRef(ABreakPlayer* Ref);
};
