// Fill out your copyright notice in the Description page of Project Settings.


#include "TTT_MinimaxPlayer.h"
#include "TTT_GameMode.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ATTT_MinimaxPlayer::ATTT_MinimaxPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GameInstance = Cast<UTTT_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

}

// Called when the game starts or when spawned
void ATTT_MinimaxPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATTT_MinimaxPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATTT_MinimaxPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATTT_MinimaxPlayer::OnTurn()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AI (Minimax) Turn"));
	GameInstance->SetTurnMessage(TEXT("AI (Minimax) Turn"));

	FTimerHandle TimerHandle;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
		{
			ATTT_GameMode* GameMode = (ATTT_GameMode*)(GetWorld()->GetAuthGameMode());

			FVector2D BestMove = FindBestMove(GameMode->GField->TileMap);
			FVector Location = GameMode->GField->GetRelativeLocationByXYPosition(BestMove.X, BestMove.Y);
			GameMode->GField->TileMap[BestMove]->SetTileStatus(PlayerNumber, ETileStatus::OCCUPIED);
			GameMode->SetCellSign(PlayerNumber, Location);

		}, 3, false);
}

void ATTT_MinimaxPlayer::OnWin()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AI (Minimax) Wins!"));
	GameInstance->SetTurnMessage(TEXT("AI Wins!"));
	GameInstance->IncrementScoreAiPlayer();
}

void ATTT_MinimaxPlayer::OnLose()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AI (Minimax) Loses!"));
	// GameInstance->SetTurnMessage(TEXT("AI Loses!"));
}

// HumanPlayer = 0
// AIPlayer = 1
int32 ATTT_MinimaxPlayer::EvaluateGrid(TMap<FVector2D, ATile*>& Board)
{
	// Checking for Rows for HumanPlayer (0) or AIPlayer (1) victory.  
	for (auto row = 0; row < 3; row++)
	{
		if ((Board[FVector2D(row, 0)])->GetOwner() == (Board[FVector2D(row, 1)])->GetOwner() &&
			(Board[FVector2D(row, 1)])->GetOwner() == (Board[FVector2D(row, 2)])->GetOwner())
		{
			// AI player
			if ((Board[FVector2D(row, 0)])->GetOwner() == 1)
			{
				return 10;
			}
			// Human player
			else if ((Board[FVector2D(row, 0)])->GetOwner() == 0)
			{
				return -10;
			}

		}
	}

	// Checking for Columns for HumanPlayer (0) or AIPlayer (1) victory.
	for (auto col = 0; col < 3; col++)
	{
		if ((Board[FVector2D(0, col)])->GetOwner() == (Board[FVector2D(1, col)])->GetOwner() &&
			(Board[FVector2D(1, col)])->GetOwner() == (Board[FVector2D(2, col)])->GetOwner())
		{
			// AI player
			if ((Board[FVector2D(0, col)])->GetOwner() == 1)
			{
				return 10;
			}
			// Human player
			else if ((Board[FVector2D(0, col)])->GetOwner() == 0)
			{
				return -10;
			}
		}
	}
	// Checking for Diagonals for HumanPlayer (0) or AIPlayer (1) victory.
	if ((Board[FVector2D(0, 0)])->GetOwner() == (Board[FVector2D(1, 1)])->GetOwner() &&
		(Board[FVector2D(1, 1)])->GetOwner() == (Board[FVector2D(2, 2)])->GetOwner())
	{
		// AI player
		if ((Board[FVector2D(0, 0)])->GetOwner() == 1)
		{
			return 10;
		}
		// Human player	
		else if ((Board[FVector2D(0, 0)])->GetOwner() == 0)
		{
			return -10;
		}

	}

	if ((Board[FVector2D(0, 2)])->GetOwner() == (Board[FVector2D(1, 1)])->GetOwner() &&
		(Board[FVector2D(1, 1)])->GetOwner() == (Board[FVector2D(2, 0)])->GetOwner())
	{
		// AI player
		if ((Board[FVector2D(0, 2)])->GetOwner() == 1)
		{
			return 10;
		}
		// Human player	
		else if ((Board[FVector2D(0, 2)])->GetOwner() == 0)
		{
			return -10;
		}

	}
	return 0;
}

// This function returns true if there are moves
// remaining on the board. It returns false if
// there are no moves left to play.
bool ATTT_MinimaxPlayer::IsMovesLeft(TMap<FVector2D, ATile*>& Board)
{
	for (const TPair<FVector2D, ATile*>& CurrPair : Board)
	{
		if ((CurrPair.Value)->GetTileStatus() == ETileStatus::EMPTY)
		{
			return true;
		}
	}
	return false;
}

int32 ATTT_MinimaxPlayer::MiniMax(TMap<FVector2D, ATile*>& Board, int32 Depth, bool IsMax)
{
	int score = EvaluateGrid(Board);

	// If Maximizer has won the game return his/her
	// evaluated score
	if (score == 10)
	{
		return score;
	}

	// If Minimizer has won the game return his/her
	// evaluated score
	if (score == -10)
	{
		return score;
	}

	// If there are no more moves and no winner then
	// it is a tie
	if (IsMovesLeft(Board) == false)
	{
		return 0;
	}

	// If this maximizer's move
	if (IsMax)
	{
		int32 best = -1000;

		// Traverse all cells
		for (auto i = 0; i < 3; i++)
		{
			for (auto j = 0; j < 3; j++)
			{
				// Check if cell is empty
				if ((Board[FVector2D(i, j)])->GetTileStatus() == ETileStatus::EMPTY)
				{
					// Make the move (set the AI player owner)
					(Board[FVector2D(i, j)])->SetTileStatus(1, ETileStatus::OCCUPIED);

					// Call minimax recursively and choose
					// the maximum value
					best = FMath::Max(best,
						MiniMax(Board, Depth + 1, !IsMax));

					// Undo the move
					(Board[FVector2D(i, j)])->SetTileStatus(-1, ETileStatus::EMPTY);
				}
			}
		}
		return best;
	}

	// If this minimizer's move
	else
	{
		int32 best = 1000;

		// Traverse all cells
		for (auto i = 0; i < 3; i++)
		{
			for (auto j = 0; j < 3; j++)
			{
				// Check if cell is empty
				if ((Board[FVector2D(i, j)])->GetTileStatus() == ETileStatus::EMPTY)
				{
					// Make the move (set the Human player owner)
					(Board[FVector2D(i, j)])->SetTileStatus(0, ETileStatus::OCCUPIED);

					// Call minimax recursively and choose
					// the minimum value
					best = FMath::Min(best,
						MiniMax(Board, Depth + 1, !IsMax));

					// Undo the move
					(Board[FVector2D(i, j)])->SetTileStatus(-1, ETileStatus::EMPTY);
				}
			}
		}
		return best;
	}
}

FVector2D ATTT_MinimaxPlayer::FindBestMove(TMap<FVector2D, ATile*>& Board)
{
	int32 bestVal = -1000;
	FVector2D bestMove;
	bestMove.X = -1;
	bestMove.Y = -1;

	// Traverse all cells, evaluate minimax function for
	// all empty cells. And return the cell with optimal
	// value.
	for (auto i = 0; i < 3; i++)
	{
		for (auto j = 0; j < 3; j++)
		{
			// Check if cell is empty
			if ((Board[FVector2D(i, j)])->GetTileStatus() == ETileStatus::EMPTY)
			{
				// Make the move (set the AI player owner)
				(Board[FVector2D(i, j)])->SetTileStatus(1, ETileStatus::OCCUPIED);

				// compute evaluation function for this
				// move.
				int32 moveVal = MiniMax(Board, 0, false);

				// Undo the move
				(Board[FVector2D(i, j)])->SetTileStatus(-1, ETileStatus::EMPTY);

				// If the value of the current move is
				// more than the best value, then update
				// best/
				if (moveVal > bestVal)
				{
					bestMove.X = i;
					bestMove.Y = j;
					bestVal = moveVal;
				}
			}
		}
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("AI (Minimax) bestVal = %d "), bestVal));


	return bestMove;
}

