// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerArmyPawn.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include <Camera/CameraComponent.h>
#include "DrawDebugHelpers.h"
#include <ArmyOfOne/TileActor.h>
#include <ArmyOfOne/UnitPawn.h>
#include <ArmyOfOne/MapData/MapManager.h>

APlayerArmyPawn::APlayerArmyPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	Camera->SetupAttachment(RootComponent);
}

void APlayerArmyPawn::BeginPlay()
{
	Super::BeginPlay();

	if (Units.Num() > 0)
		SelectedTile = Units[0]->CurrentTile;
}

void APlayerArmyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveTimer += DeltaTime;

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		FVector Start, Dir, End;
		PC->DeprojectMousePositionToWorld(Start, Dir);
		End = Start + (Dir * 8000.0f);
		TraceForActor(Start, End, false);
	}

	ATileActor* tile = Cast<ATileActor>(TracedActor);
	if (tile)
	{
		if (SelectedTile != tile)
		{
			SelectedTile = tile;
			ShouldUpdateRange = true;
		}
	}
	else
	{
		AUnitPawn* unit = Cast<AUnitPawn>(TracedActor);
		if (unit)
		{
			if (SelectedTile != unit->CurrentTile)
			{
				SelectedTile = unit->CurrentTile;
				ShouldUpdateRange = true;
			}
			
		}
	}


	if (ShouldUpdateRange)
	{
		if (SelectedUnit)
		{
			if (SelectedUnit->CurrentState == EUnitState::SELECTED)
				ShowMoveRange(SelectedUnit);
			else if (SelectedUnit->CurrentState == EUnitState::MOVED)
				ShowAttackRange(SelectedUnit);
			ShouldUpdateRange = false;
		}
		else if (SelectedTile->Unit)
		{
			if (SelectedTile->Unit->CurrentState == EUnitState::READY)
				ShowMoveRange(SelectedTile->Unit);
			ShouldUpdateRange = false;
		}
		else
			ClearMapRange();
	}
}

void APlayerArmyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("LeftClick", EInputEvent::IE_Pressed, this, &APlayerArmyPawn::OnMouseClick);
	PlayerInputComponent->BindAction("Select", EInputEvent::IE_Pressed, this, &APlayerArmyPawn::SelectTile);
	PlayerInputComponent->BindAction("Cancel", EInputEvent::IE_Pressed, this, &APlayerArmyPawn::CancelSelection);

	//Input for stick
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerArmyPawn::MoveGridHorizontal);
	PlayerInputComponent->BindAxis("MoveUp", this, &APlayerArmyPawn::MoveGridVertical);

	//Inputs for D-Pad
	PlayerInputComponent->BindAction("DPadUp", EInputEvent::IE_Pressed, this, &APlayerArmyPawn::MoveUp);
	PlayerInputComponent->BindAction("DPadDown", EInputEvent::IE_Pressed, this, &APlayerArmyPawn::MoveDown);
	PlayerInputComponent->BindAction("DPadLeft", EInputEvent::IE_Pressed, this, &APlayerArmyPawn::MoveLeft);
	PlayerInputComponent->BindAction("DPadRight", EInputEvent::IE_Pressed, this, &APlayerArmyPawn::MoveRight);

	
}

void APlayerArmyPawn::MoveUp()
{
	int x = SelectedTile->HorizontalIndex;
	int y = SelectedTile->VerticalIndex;

	if(MapData->GetTile(x, y + 1))
		SelectedTile = MapData->GetTile(x, y + 1);

	ShouldUpdateRange = true;
}

void APlayerArmyPawn::MoveDown()
{
	int x = SelectedTile->HorizontalIndex;
	int y = SelectedTile->VerticalIndex;

	if (MapData->GetTile(x, y - 1))
		SelectedTile = MapData->GetTile(x, y - 1);

	ShouldUpdateRange = true;
}

void APlayerArmyPawn::MoveLeft()
{
	int x = SelectedTile->HorizontalIndex;
	int y = SelectedTile->VerticalIndex;

	if (MapData->GetTile(x - 1, y))
		SelectedTile = MapData->GetTile(x - 1, y);

	ShouldUpdateRange = true;
}

void APlayerArmyPawn::MoveRight()
{
	int x = SelectedTile->HorizontalIndex;
	int y = SelectedTile->VerticalIndex;

	if (MapData->GetTile(x + 1, y))
		SelectedTile = MapData->GetTile(x + 1, y);

	ShouldUpdateRange = true;
}

void APlayerArmyPawn::MoveGridHorizontal(float direction)
{
	if (MoveTimer < MoveDelay || FMath::Abs(direction) < Sensitivity)
		return;

	int x = SelectedTile->HorizontalIndex;
	int y = SelectedTile->VerticalIndex;

	if (direction > Sensitivity)
		MoveRight();
	else if (direction < -Sensitivity)
		MoveLeft();
	MoveTimer = 0.0f;
}

void APlayerArmyPawn::MoveGridVertical(float direction)
{
	if (MoveTimer < MoveDelay || FMath::Abs(direction) < Sensitivity)
		return;

	int x = SelectedTile->HorizontalIndex;
	int y = SelectedTile->VerticalIndex;

	if (direction > Sensitivity)
		MoveUp();
	else if (direction < -Sensitivity)
		MoveDown();
	MoveTimer = 0.0f;
}

void APlayerArmyPawn::SelectTile()
{
	if (!SelectedTile)
		return;

	if (SelectedTile->Unit)
	{
		if (SelectedUnit)
		{
			if (SelectedTile->Unit == SelectedUnit)
				SelectedUnit->CurrentState = EUnitState::MOVED;
			else if(SelectedUnit->IsOpposing(SelectedTile->Unit))
			{
				TArray<ATileActor*> nearbyTiles = SelectedUnit->GetAttackableTiles(SelectedTile);
				ATileActor* closestTile = SelectedTile;

				for (int i = 0; i < nearbyTiles.Num(); i++)
				{
					if (AMapManager::GetSmallestDistance(SelectedUnit->CurrentTile, nearbyTiles[i]) < AMapManager::GetSmallestDistance(SelectedUnit->CurrentTile, closestTile))
						closestTile = nearbyTiles[i];
				}

				if (AMapManager::GetSmallestDistance(SelectedUnit->CurrentTile, closestTile) <= SelectedUnit->MovementRange)
					SelectedUnit->MoveToTile(closestTile);
			}
		}
		else //There is no unit currently selected.
		{
			SelectedUnit = SelectedTile->Unit;
			SelectedUnit->CurrentState = EUnitState::SELECTED;
			ShouldUpdateRange = true;
		}
	}
	else if(SelectedUnit)
	{
		if (AMapManager::GetSmallestDistance(SelectedUnit->CurrentTile, SelectedTile) <= SelectedUnit->MovementRange)
			SelectedUnit->MoveToTile(SelectedTile);
	}
}

void APlayerArmyPawn::CancelSelection()
{
	if (!SelectedUnit)
		return;

	SelectedUnit->CurrentState = EUnitState::READY;
	SelectedUnit->MovementPath.Empty();

	SelectedUnit->CurrentTile->Unit = nullptr;
	SelectedUnit->StartTile->Unit = SelectedUnit;
	SelectedUnit->CurrentTile = SelectedUnit->StartTile;
	SelectedUnit->SetActorLocation(SelectedUnit->StartTile->GetActorLocation() + FVector(0, 0, 25.0f));

	SelectedTile = SelectedUnit->StartTile;
	SelectedUnit = nullptr;
	ShouldUpdateRange = true;
}

void APlayerArmyPawn::ClearMapRange()
{
	TArray<ATileActor*> map = MapData->GridMap;

	for (int i = 0; i < map.Num(); i++)
	{
		if (map[i]->MovementMesh->GetVisibleFlag())
			map[i]->MovementMesh->SetVisibility(false);
	}
}

void APlayerArmyPawn::ShowMoveRange(AUnitPawn* unit)
{
	if (!unit)
		return;

	ClearMapRange();
	TArray<ATileActor*> nearTile = unit->GetAllTilesInRange();

	UMaterialInterface* moveMat = LoadObject<UMaterialInterface>(NULL, TEXT("MaterialInstanceConstant'/Game/Materials/MovementIndicators/MI_MovementIndicator_Blue.MI_MovementIndicator_Blue'"));
	UMaterialInterface* attackMat = LoadObject<UMaterialInterface>(NULL, TEXT("MaterialInstanceConstant'/Game/Materials/MovementIndicators/MI_MovementIndicator_Red.MI_MovementIndicator_Red'"));
	for (int i = 0; i < nearTile.Num(); i++)
	{
		nearTile[i]->MovementMesh->SetVisibility(true);

		if (nearTile[i]->Unit)
		{
			if(nearTile[i]->Unit != unit)
				nearTile[i]->MovementMesh->SetMaterial(0, attackMat);
			else
				nearTile[i]->MovementMesh->SetMaterial(0, moveMat);
		}
		else
			nearTile[i]->MovementMesh->SetMaterial(0, moveMat);

		TArray<ATileActor*> attackTile = unit->GetAttackableTiles(nearTile[i]);

		for (int j = 0; j < attackTile.Num(); j++)
		{
			if (!nearTile.Contains(attackTile[j]))
			{
				attackTile[j]->MovementMesh->SetVisibility(true);
				attackTile[j]->MovementMesh->SetMaterial(0, attackMat);
			}
		}
	}
}

void APlayerArmyPawn::ShowAttackRange(AUnitPawn* unit)
{
	if (!unit)
		return;

	ClearMapRange();
	TArray<ATileActor*> nearTile = unit->GetAttackableTiles();

	UMaterialInterface* moveMat = LoadObject<UMaterialInterface>(NULL, TEXT("MaterialInstanceConstant'/Game/Materials/MovementIndicators/MI_MovementIndicator_Blue.MI_MovementIndicator_Blue'"));
	UMaterialInterface* attackMat = LoadObject<UMaterialInterface>(NULL, TEXT("MaterialInstanceConstant'/Game/Materials/MovementIndicators/MI_MovementIndicator_Red.MI_MovementIndicator_Red'"));

	for (int i = 0; i < nearTile.Num(); i++)
	{
		nearTile[i]->MovementMesh->SetVisibility(true);
		nearTile[i]->MovementMesh->SetMaterial(0, attackMat);
	}

	unit->CurrentTile->MovementMesh->SetMaterial(0, moveMat);
}

void APlayerArmyPawn::TraceForActor(const FVector& start, const FVector& end, bool DrawDebugHelpers)
{
	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, start, end, ECC_Visibility);
	if (DrawDebugHelpers)
	{
		DrawDebugLine(GetWorld(), start, Hit.Location, FColor::Red);
		DrawDebugSolidBox(GetWorld(), Hit.Location, FVector(20.0f), FColor::Red);
	}
	if (Hit.Actor.IsValid())
		TracedActor = Hit.Actor.Get();
	else
		TracedActor = nullptr;
}

void APlayerArmyPawn::OnMouseClick()
{
	ATileActor* tile = Cast<ATileActor>(TracedActor);
	if (tile)
		SelectedTile = tile;
	else
	{
		AUnitPawn* unit = Cast<AUnitPawn>(TracedActor);
		if(unit)
			SelectedTile = unit->CurrentTile;
	}

	SelectTile();
}