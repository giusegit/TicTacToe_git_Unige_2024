// Fill out your copyright notice in the Description page of Project Settings.


#include "GameField.h"
#include "Kismet/GameplayStatics.h"
#include "BaseSign.h"
#include "TTT_GameMode.h"

// Sets default values
AGameField::AGameField()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	// size of winnning line
	WinSize = 3;
	// size of the field (3x3)
	Size = 3;
	// tile dimension
	TileSize = 120.f;
	// tile padding percentage 
	CellPadding =  0.2f;
	

}

void AGameField::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	//normalized tilepadding
	NormalizedCellPadding = (TileSize + TileSize * CellPadding) / TileSize; 
}

// Called when the game starts or when spawned
void AGameField::BeginPlay()
{
	Super::BeginPlay();
	GenerateField();
}

void AGameField::ResetField()
{
	for (ATile* Obj : TileArray)
	{
		Obj->SetTileStatus(NOT_ASSIGNED, ETileStatus::EMPTY);
	}

	// send broadcast event to registered objects 
	OnResetEvent.Broadcast();

	ATTT_GameMode* GameMode = Cast<ATTT_GameMode>(GetWorld()->GetAuthGameMode());
	GameMode->IsGameOver = false;
	GameMode->MoveCounter = 0;
	GameMode->ChoosePlayerAndStartGame();
}

void AGameField::GenerateField()
{
	for (int32 IndexX = 0; IndexX < Size; IndexX++)
	{
		for (int32 IndexY = 0; IndexY < Size; IndexY++)
		{
			FVector Location = AGameField::GetRelativeLocationByXYPosition(IndexX, IndexY);
			ATile* Obj = GetWorld()->SpawnActor<ATile>(TileClass, Location, FRotator::ZeroRotator);
			const float TileScale = TileSize / 100.f;
			const float Zscaling = 0.2f;
			Obj->SetActorScale3D(FVector(TileScale, TileScale, Zscaling));
			Obj->SetGridPosition(IndexX, IndexY);
			TileArray.Add(Obj);
			TileMap.Add(FVector2D(IndexX, IndexY), Obj);
		}
	}
}

FVector2D AGameField::GetPosition(const FHitResult& Hit)
{
	return Cast<ATile>(Hit.GetActor())->GetGridPosition();
}

TArray<ATile*>& AGameField::GetTileArray()
{
	return TileArray;
}

FVector AGameField::GetRelativeLocationByXYPosition(const int32 InX, const int32 InY) const
{
	return TileSize * NormalizedCellPadding * FVector(InX, InY, 0);
}

FVector2D AGameField::GetXYPositionByRelativeLocation(const FVector& Location) const
{
	const double XPos = Location.X / (TileSize * NormalizedCellPadding);
	const double YPos = Location.Y / (TileSize * NormalizedCellPadding);
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("x=%f,y=%f"), XPos, YPos));
	return FVector2D(XPos, YPos);
}

bool AGameField::IsWinPosition(const FVector2D Position) const
{
	const int32 Offset = WinSize - 1;
	// vertical lines
	for (int32 IndexI = Position.X - Offset; IndexI <= Position.X; IndexI++)
	{
		if (IsWinLine(FVector2D(IndexI, Position.Y), FVector2D(IndexI + Offset, Position.Y)))
		{
			return true;
		}
	}

	// horizontal lines
	for (int32 IndexI = Position.Y - Offset; IndexI <= Position[1]; IndexI++)
	{
		if (IsWinLine(FVector2D(Position.X, IndexI), FVector2D(Position.X, IndexI + Offset)))
		{
			return true;
		}
	}

	// diagonal lines
	for (int32 IndexI = -Offset; IndexI <= 0; IndexI++)
	{
		if (IsWinLine(FVector2D(Position.X + IndexI, Position.Y + IndexI), FVector2D(Position.X + Offset + IndexI, Position.Y + Offset + IndexI)))
		{
			return true;
		}
		if (IsWinLine(FVector2D(Position.X + IndexI, Position.Y - IndexI), FVector2D(Position.X + Offset + IndexI, Position.Y - Offset - IndexI)))
		{
			return true;
		}
	}

	return false;
}

inline bool AGameField::IsWinLine(const FVector2D Begin, const FVector2D End) const
{
	return IsValidPosition(Begin) && IsValidPosition(End) && AllEqual(GetLine(Begin, End));
}

inline bool AGameField::IsValidPosition(const FVector2D Position) const
{
	return 0 <= Position.X && Position.X < Size && 0 <= Position.Y && Position.Y < Size;
}

TArray<int32> AGameField::GetLine(const FVector2D Begin, const FVector2D End) const
{
	int32 XSign;
	if (Begin.X == End.X)
	{
		XSign = 0;
	}
	else
	{
		XSign = Begin.X < End.X ? 1 : -1;
	}

	int32 YSign;
	if (Begin.Y == End.Y)
	{
		YSign = 0;
	}
	else
	{
		YSign = Begin.Y < End.Y ? 1 : -1;
	}

	TArray<int32> Line;
	int32 XVal = Begin.X - XSign;
	int32 YVal = Begin.Y - YSign;
	do 
	{
		XVal += XSign;
		YVal += YSign;
		Line.Add((TileMap[FVector2D(XVal, YVal)])->GetOwner());
	} while (XVal != End.X || YVal != End.Y);

	return Line;
}

bool AGameField::AllEqual(const TArray<int32>& Array) const
{
	if (Array.Num() == 0)
	{
		return false;
	}
	const int32 Value = Array[0];

	if (Value == NOT_ASSIGNED)
	{
		return false;
	}

	for (int32 IndexI = 1; IndexI < Array.Num(); IndexI++)
	{
		if (Value != Array[IndexI])
		{
			return false;
		}
	}

	return true;
}

// Called every frame
//void AGameField::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

