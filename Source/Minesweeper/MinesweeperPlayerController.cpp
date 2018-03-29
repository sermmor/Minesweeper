// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "MinesweeperPlayerController.h"

AMinesweeperPlayerController::AMinesweeperPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	ClickEventKeys.Add(EKeys::RightMouseButton);
}
