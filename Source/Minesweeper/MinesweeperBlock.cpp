// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "MinesweeperBlock.h"
#include "MinesweeperBlockGrid.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstance.h"

AMinesweeperBlock::AMinesweeperBlock() : MineValue(0), IsMarkedAsMineByPlayer(false)
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		ConstructorHelpers::FObjectFinderOptional<UMaterial> BaseMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> BlueMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> OrangeMaterial;
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Puzzle/Meshes/PuzzleCube.PuzzleCube"))
			, BaseMaterial(TEXT("/Game/Puzzle/Meshes/BaseMaterial.BaseMaterial"))
			, BlueMaterial(TEXT("/Game/Puzzle/Meshes/BlueMaterial.BlueMaterial"))
			, OrangeMaterial(TEXT("/Game/Puzzle/Meshes/OrangeMaterial.OrangeMaterial"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh0"));
	BlockMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());
	//BlockMesh->SetRelativeScale3D(FVector(1.f,1.f,0.25f));
	BlockMesh->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.25f));
	BlockMesh->SetRelativeLocation(FVector(0.f,0.f,25.f));
	BlockMesh->SetMaterial(0, ConstructorStatics.BlueMaterial.Get());
	BlockMesh->SetupAttachment(DummyRoot);
	BlockMesh->OnClicked.AddDynamic(this, &AMinesweeperBlock::BlockClicked);
	BlockMesh->OnInputTouchBegin.AddDynamic(this, &AMinesweeperBlock::OnFingerPressedBlock);

	// Save a pointer to the orange material
	BaseMaterial = ConstructorStatics.BaseMaterial.Get();
	BlueMaterial = ConstructorStatics.BlueMaterial.Get();
	OrangeMaterial = ConstructorStatics.OrangeMaterial.Get();

	// CREATE NUMBER.

	// Create static mesh component
	MineText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("MineText"));
	//MineText->SetRelativeLocation(FVector(50.f, 50.f, 100.0f));
	MineText->SetRelativeLocation(FVector(-50.f, -50.f, 0.0f));
	MineText->SetRelativeRotation(FRotator(90.f, 180.f, 0.f));
	MineText->SetXScale(5.0f);
	MineText->SetYScale(5.0f);
	MineText->SetText("");
	MineText->SetupAttachment(DummyRoot);
}

void AMinesweeperBlock::BlockClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked)
{
	if (ButtonClicked == EKeys::LeftMouseButton)
		HandleClicked();
	else if (ButtonClicked == EKeys::RightMouseButton && OwningGrid != nullptr && !OwningGrid->IsInGameOver())
	{
		if (IsMarkedAsMineByPlayer)
		{
			BlockMesh->SetMaterial(0, BlueMaterial);
			bIsActive = false;
		}
		else
		{
			BlockMesh->SetMaterial(0, OrangeMaterial);
			bIsActive = true;
		}
		IsMarkedAsMineByPlayer = !IsMarkedAsMineByPlayer;
	}
}


void AMinesweeperBlock::OnFingerPressedBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
	HandleClicked();
}

void AMinesweeperBlock::HandleClicked()
{
	// Check we are not already active
	if (!bIsActive && OwningGrid != nullptr && !OwningGrid->IsInGameOver())
	{
		bIsActive = true;

		// Destroy Mesh.
		BlockMesh->DestroyComponent();

		// Show Mine value.
		ShowMineValue();

		// Tell the Grid
		if (MineValue == -1)
			OwningGrid->SetGameOverState();
		else
			OwningGrid->AddScore();
	}
}

void AMinesweeperBlock::Highlight(bool bOn)
{
	// Do not highlight if the block has already been activated.
	if (bIsActive)
	{
		return;
	}

	if (bOn)
	{
		BlockMesh->SetMaterial(0, BaseMaterial);
	}
	else
	{
		BlockMesh->SetMaterial(0, BlueMaterial);
	}
}

void AMinesweeperBlock::SetMineValue(int newMineValue)
{
	MineValue = newMineValue;
}

void AMinesweeperBlock::ShowMineValue()
{
	MineText->SetText(FText::AsNumber(MineValue));
}
