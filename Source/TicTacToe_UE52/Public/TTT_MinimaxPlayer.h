// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TTT_PlayerInterface.h"
#include "TTT_GameInstance.h"
#include "Tile.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "TTT_MinimaxPlayer.generated.h"

UCLASS()
class TICTACTOE_UE52_API ATTT_MinimaxPlayer : public APawn, public ITTT_PlayerInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATTT_MinimaxPlayer();

	UTTT_GameInstance* GameInstance;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void OnTurn() override;
	virtual void OnWin() override;
	virtual void OnLose() override;

	int32 EvaluateGrid(TMap<FVector2D, ATile*>& Board);
	bool IsMovesLeft(TMap<FVector2D, ATile*>& Board);
	int32 MiniMax(TMap<FVector2D, ATile*>& Board, int32 Depth, bool IsMax);
	FVector2D FindBestMove(TMap<FVector2D, ATile*>& Board);
};
