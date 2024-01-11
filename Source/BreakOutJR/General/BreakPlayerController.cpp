// Fill out your copyright notice in the Description page of Project Settings.


#include "General/BreakPlayerController.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/TimelineComponent.h"

#include "Player/BreakPlayer.h"
#include "Components/BreakHealthComponent.h"
#include "General/BreakGameStateBase.h"
#include "Widgets/BreakTextDisplayerWidget.h"


ABreakPlayerController::ABreakPlayerController()
{
	GameOverScreenOpacityTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("GameOverScreenOpacityTimeline"));
}

void ABreakPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FindLevelCamera();
	CreateWidgets();
}

void ABreakPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (InputComponent)
	{
		InputComponent->BindAxis("MoveRight", this, &ABreakPlayerController::MovePaddleRight);

		InputComponent->BindAction("Launch", EInputEvent::IE_Pressed, this, &ABreakPlayerController::LaunchBall);
	}
}

void ABreakPlayerController::MovePaddleRight(float AxisValue)
{
	if (!BreakPlayerRef || !bCanReceiveInput) return;

	BreakPlayerRef->MovePaddleRight(AxisValue);
}

void ABreakPlayerController::LaunchBall()
{
	if (!BreakPlayerRef || !bCanReceiveInput) return;
	
	BreakPlayerRef->LaunchBall();
}

void ABreakPlayerController::SetBreakGameStateRef(ABreakGameStateBase* Ref)
{
	BreakGameStateRef = Ref;
	if (!BreakGameStateRef) return;

	BreakGameStateRef->IsGameOverDel.BindUObject(this, &ABreakPlayerController::StartGameOverEvents);
}

void ABreakPlayerController::SetBreakPlayerRef(ABreakPlayer* Ref)
{
	BreakPlayerRef = Ref;
	if (!BreakPlayerRef) return;

	PlayerHealthCompRef = BreakPlayerRef->GetHealthComponent();

	BreakPlayerRef->AllBallsLostDel.AddUObject(this, &ABreakPlayerController::UpdatePlayerLivesWidget);
}

void ABreakPlayerController::FindLevelCamera()
{
	AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ACameraActor::StaticClass());
	if (!FoundActor) return;

	SetViewTargetWithBlend(FoundActor);
}

void ABreakPlayerController::CreateWidgets()
{
	if (HUDOverlayAsset) HUDOverlayRef = CreateWidget<UUserWidget>(this, HUDOverlayAsset);
	if (HUDOverlayRef)
	{
		HUDOverlayRef->AddToViewport();
		HUDOverlayRef->SetVisibility(ESlateVisibility::Visible);

		PlayerScoreTextRef = HUDOverlayRef->WidgetTree->FindWidget<UBreakTextDisplayerWidget>(TEXT("WBP_PlayerScoreText"));
		PlayerLivesTextRef = HUDOverlayRef->WidgetTree->FindWidget<UBreakTextDisplayerWidget>(TEXT("WBP_PlayerLivesText"));

		UpdatePlayerScoreWidget(0);
		UpdatePlayerLivesWidget();
	}

	if (GameOverScreenAsset) GameOverScreenRef = CreateWidget<UBreakTextDisplayerWidget>(this, GameOverScreenAsset);
	if (GameOverScreenRef)
	{
		GameOverScreenRef->AddToViewport();
		GameOverScreenRef->SetVisibility(ESlateVisibility::Hidden);
		UpdateGameOverScreenOpacity(0.0f);

		if (GameOverScreenOpacityFloatCurve)
		{
			FOnTimelineFloat UpdateOpacityTimelineFloatVal;

			//Binding our float track to our UpdateTimelineComp Function's output
			UpdateOpacityTimelineFloatVal.BindDynamic(this, &ABreakPlayerController::UpdateGameOverScreenOpacity);

			//If we have a float curve, bind it's graph to our update function
			GameOverScreenOpacityTimeline->AddInterpFloat(GameOverScreenOpacityFloatCurve, UpdateOpacityTimelineFloatVal);
		}
	}
}

void ABreakPlayerController::StartGameOverEvents(bool bHasPlayerWon)
{
	bCanReceiveInput = false;

	if(BreakPlayerRef) BreakPlayerRef->MovePaddleRight(0.0f);

	DisplayGameOverScreen(bHasPlayerWon);
}

void ABreakPlayerController::DisplayGameOverScreen(bool bHasPlayerWon)
{
	if (!HUDOverlayRef || !GameOverScreenRef || !GameOverScreenOpacityTimeline) return;

	HUDOverlayRef->SetVisibility(ESlateVisibility::Hidden);
	GameOverScreenRef->SetVisibility(ESlateVisibility::Visible);

	FString GameOverText = FString::Printf(TEXT("%s"), bHasPlayerWon ? TEXT("YOU WON!") : TEXT("TRY AGAIN!"));
	GameOverScreenRef->SetTextToDisplay(GameOverText);

	GameOverScreenOpacityTimeline->PlayFromStart();
}

void ABreakPlayerController::UpdateGameOverScreenOpacity(float value)
{
	if (!GameOverScreenRef) return;
	
	GameOverScreenRef->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, value)); //RGBA
}

void ABreakPlayerController::UpdatePlayerScoreWidget(const int32 NewScore)
{
	if (!PlayerScoreTextRef) return;

	FString ScoreText = FString::Printf(TEXT("SCORE: %d"), NewScore);
	PlayerScoreTextRef->SetTextToDisplay(ScoreText);
}

void ABreakPlayerController::UpdatePlayerLivesWidget()
{
	if (!PlayerHealthCompRef || !PlayerLivesTextRef) return;

	FString LivesText = FString::Printf(TEXT("LIVES: %d"), PlayerHealthCompRef->GetCurrentHealth());
	PlayerLivesTextRef->SetTextToDisplay(LivesText);
}