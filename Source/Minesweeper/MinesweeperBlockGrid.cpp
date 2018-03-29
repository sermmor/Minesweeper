// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "MinesweeperBlockGrid.h"
#include "MinesweeperBlock.h"
#include "Components/TextRenderComponent.h"
#include "Engine/World.h"

#define LOCTEXT_NAMESPACE "PuzzleBlockGrid"

AMinesweeperBlockGrid::AMinesweeperBlockGrid() : IsInGameOverState(false)
{
	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	ScoreText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ScoreText0"));
	ScoreText->SetRelativeLocation(FVector(200.f,0.f,0.f));
	ScoreText->SetRelativeRotation(FRotator(90.f,0.f,0.f));
	ScoreText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Score: {0}"), FText::AsNumber(0)));
	ScoreText->SetupAttachment(DummyRoot);

	// Set defaults
	Size = 8;
	BlockSpacing = 150.0f;

	CreateSequenceBlockValues();
}

void AMinesweeperBlockGrid::CreateRandomMinesPositions()
{
	int candidate;
	bool isFindedNewPos = false;
	for (int i = 0; i < NUMBER_MINES; i++)
	{
		while (!isFindedNewPos)
		{
			// Search new random but not repead positions.
			isFindedNewPos = true;
			candidate = FMath::RandRange(0, SIZE_SECUENCE - 1);
			for (int j = 0; j < i; j++)
			{
				if (minesPositions[j] == candidate)
				{
					isFindedNewPos = false;
					break;
				}
			}
		}
		minesPositions[i] = candidate;
		isFindedNewPos = false;
	}
}

bool AMinesweeperBlockGrid::IsMinePosition(int position)
{
	bool isFinded = false;
	for (int i = 0; i < NUMBER_MINES; i++)
	{
		if (minesPositions[i] == position)
		{
			isFinded = true;
			break;
		}
	}
	return isFinded;
}

int AMinesweeperBlockGrid::NumberRow(int indexBlock)
{
	return indexBlock / SIZE_SQUARE;
}

int AMinesweeperBlockGrid::NumberCol(int indexBlock)
{
	return indexBlock % SIZE_SQUARE;
}

int AMinesweeperBlockGrid::CalcIndexBlock(int row, int col)
{
	return (SIZE_SQUARE * row) + col;
}

bool AMinesweeperBlockGrid::IsAdjMinePosition(int position)
{
	int row = NumberRow(position), rowMine;
	int col = NumberCol(position), colMine;

	for (int i = 0; i < NUMBER_MINES; i++)
	{
		rowMine = NumberRow(minesPositions[i]);
		colMine = NumberCol(minesPositions[i]);
		
		if (((row == rowMine - 1) && (col == colMine - 1)) || ((row == rowMine - 1) && (col == colMine))
				|| ((row == rowMine - 1) && (col == colMine + 1)) || ((row == rowMine) && (col == colMine - 1))
				|| ((row == rowMine) && (col == colMine + 1)) || ((row == rowMine + 1) && (col == colMine - 1))
				|| ((row == rowMine + 1) && (col == colMine)) || ((row == rowMine + 1) && (col == colMine + 1)))
			return true;
	}

	return false;
}

int AMinesweeperBlockGrid::NumberAdjMinePosition(int position)
{
	int row = NumberRow(position), rowMine;
	int col = NumberCol(position), colMine;
	int minesCounter = 0;

	for (int i = 0; i < NUMBER_MINES; i++)
	{
		rowMine = NumberRow(minesPositions[i]);
		colMine = NumberCol(minesPositions[i]);

		if (((row == rowMine - 1) && (col == colMine - 1)) || ((row == rowMine - 1) && (col == colMine))
			|| ((row == rowMine - 1) && (col == colMine + 1)) || ((row == rowMine) && (col == colMine - 1))
			|| ((row == rowMine) && (col == colMine + 1)) || ((row == rowMine + 1) && (col == colMine - 1))
			|| ((row == rowMine + 1) && (col == colMine)) || ((row == rowMine + 1) && (col == colMine + 1)))
		{
			minesCounter++;
		}
	}

	return minesCounter;
}

void AMinesweeperBlockGrid::CreateSequenceBlockValues()
{
	// Create NUMBER_MINES (10) mines in random position in array.
	CreateRandomMinesPositions();

	// Create 8 numbers arround mines (not write in a mine square).
	for (int BlockIndex = 0; BlockIndex < SIZE_SECUENCE; BlockIndex++)
	{
		if (IsMinePosition(BlockIndex))
			SecuenceBlockValue[BlockIndex] = -1;
		else if (IsAdjMinePosition(BlockIndex))
			SecuenceBlockValue[BlockIndex] = NumberAdjMinePosition(BlockIndex);
	}
}

void AMinesweeperBlockGrid::BeginPlay()
{
	Super::BeginPlay();

	// Number of blocks
	const int32 NumBlocks = Size * Size;

	// Loop to spawn each block
	for(int32 BlockIndex=0; BlockIndex<NumBlocks; BlockIndex++)
	{
		const float XOffset = (BlockIndex/Size) * BlockSpacing; // Divide by dimension
		const float YOffset = (BlockIndex%Size) * BlockSpacing; // Modulo gives remainder

		// Make position vector, offset from Grid location
		const FVector BlockLocation = FVector(XOffset, YOffset, 0.f) + GetActorLocation();

		// Spawn a block
		AMinesweeperBlock* NewBlock = GetWorld()->SpawnActor<AMinesweeperBlock>(BlockLocation, FRotator(0,0,0));

		// Tell the block about its owner
		if (NewBlock != nullptr)
		{
			NewBlock->OwningGrid = this;
			//NewBlock->SetMineValue(FMath::RandRange(-1, 8));
			NewBlock->SetMineValue(SecuenceBlockValue[BlockIndex]);
		}
	}
}


void AMinesweeperBlockGrid::AddScore()
{
	// Increment score
	Score++;

	// Update text
	ScoreText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Score: {0}"), FText::AsNumber(Score)));
}

void AMinesweeperBlockGrid::SetGameOverState()
{
	IsInGameOverState = true;
}

bool AMinesweeperBlockGrid::IsInGameOver() const
{
	return IsInGameOverState;
}

#undef LOCTEXT_NAMESPACE
