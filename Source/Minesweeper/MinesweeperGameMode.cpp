// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "MinesweeperGameMode.h"
#include "MinesweeperPlayerController.h"
#include "MinesweeperPawn.h"

AMinesweeperGameMode::AMinesweeperGameMode()
{
	// no pawn by default
	DefaultPawnClass = AMinesweeperPawn::StaticClass();
	// use our own player controller class
	PlayerControllerClass = AMinesweeperPlayerController::StaticClass();
}
