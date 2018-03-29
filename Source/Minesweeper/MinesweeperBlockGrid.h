// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MinesweeperBlockGrid.generated.h"

const int SIZE_SQUARE = 8;
const int SIZE_SECUENCE = SIZE_SQUARE * SIZE_SQUARE;
const int NUMBER_MINES = 10;

/** Class used to spawn blocks and manage score */
UCLASS(minimalapi)
class AMinesweeperBlockGrid : public AActor
{
	GENERATED_BODY()

	/** Dummy root component */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;

	/** Text component for the score */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UTextRenderComponent* ScoreText;

public:
	AMinesweeperBlockGrid();

	/** How many blocks have been clicked */
	int32 Score;

	/** Number of blocks along each side of grid */
	UPROPERTY(Category=Grid, EditAnywhere, BlueprintReadOnly)
	int32 Size;

	/** Spacing of blocks */
	UPROPERTY(Category=Grid, EditAnywhere, BlueprintReadOnly)
	float BlockSpacing;

protected:
	// Begin AActor interface
	virtual void BeginPlay() override;
	// End AActor interface

public:

	/** Handle the block being clicked */
	void AddScore();

	/** Returns DummyRoot subobject **/
	FORCEINLINE class USceneComponent* GetDummyRoot() const { return DummyRoot; }
	/** Returns ScoreText subobject **/
	FORCEINLINE class UTextRenderComponent* GetScoreText() const { return ScoreText; }

	bool IsInGameOver() const;
	void SetGameOverState();

private:
	bool IsInGameOverState;
	// Sequence block grids value.
	int SecuenceBlockValue[SIZE_SECUENCE] = { 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, };/* = { 2, 2, 1, 0, 0, 0, 0, 0,
		-1, -1, 3, 1, 0, 0, 0, 0,
		4, -1, -1, 1, 1, 1, 1, 0,
		2, -1, 3, 1, 1, -1, 2, 1,
		1, 1, 1, 0, 1, 1, 2, -1,
		1, 1, 0, 0, 1, 1, 2, 1,
		-1, 1, 0, 1, 2, -1, 1, 0,
		1, 1, 0, 1, -1, 2, 1, 0 };*/
	int minesPositions[NUMBER_MINES] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	void CreateRandomMinesPositions();
	void CreateSequenceBlockValues();
	bool IsMinePosition(int position);
	bool IsAdjMinePosition(int position);
	int NumberAdjMinePosition(int position);

	int NumberRow(int indexBlock);
	int NumberCol(int indexBlock);
	int CalcIndexBlock(int row, int col);
};



