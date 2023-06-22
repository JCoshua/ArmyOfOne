// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitPawn.h"
#include "TileActor.h"
#include "MapData/MapManager.h"
#include "ArmyPawns/PlayerArmyPawn.h"

// Sets default values
AUnitPawn::AUnitPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AUnitPawn::BeginPlay()
{
	Super::BeginPlay();
	if (CurrentTile)
		StartTile = CurrentTile;
}

// Called every frame
void AUnitPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(CurrentTile)
		SetActorLocation(FMath::Lerp(GetActorLocation(), CurrentTile->GetActorLocation() + FVector(0, 0, 25.0f), 0.1f));
	if (MovementPath.Num() > 0)
	{
		CurrentTile->Unit = nullptr;
		CurrentTile = MovementPath[0];
		if (FVector::Dist2D(GetActorLocation(), CurrentTile->GetActorLocation()) < 5.0f)
		{
			MovementPath.Remove(CurrentTile);
			CurrentTile->Unit = this;

			if (MovementPath.Num() == 0)
			{
				CurrentState = EUnitState::MOVED;
				APlayerArmyPawn* player = Cast<APlayerArmyPawn>(UnitOwner);
				if(player)
					player->ShouldUpdateRange = true;
			}
		}
	}
}

bool AUnitPawn::MoveTile(int xDir, int yDir)
{
	if (!CurrentTile)
		return false;

	int x = CurrentTile->HorizontalIndex + xDir;
	int y = CurrentTile->VerticalIndex + yDir;

	if (ATileActor* newTile = CurrentTile->MapData->GetTile(x,y))
	{
		if (newTile->Unit || newTile->MovementCost <= 0)
			return false;

		newTile->Unit = this;
		CurrentTile->Unit = nullptr;
		CurrentTile = newTile;

		return true;
	}

	return false;
}

bool AUnitPawn::MoveToTile(int x, int y)
{
	if (!CurrentTile || !CurrentTile->MapData)
		return false;

	ATileActor* target = CurrentTile->MapData->GetTile(x, y);
	if (!target)
		return false;

	MovementPath = AMapManager::GetShortestPath(CurrentTile, target);

	if (MovementPath.Num() == 0)
	{
		CurrentState = EUnitState::MOVED;
		APlayerArmyPawn* player = Cast<APlayerArmyPawn>(UnitOwner);
		if (player)
			player->ShouldUpdateRange = true;
	}
	return true;
}

bool AUnitPawn::MoveToTile(ATileActor* target)
{
	if (!CurrentTile || !CurrentTile->MapData || !target)
		return false;
	
	MovementPath = AMapManager::GetShortestPath(CurrentTile, target);

	if (MovementPath.Num() == 0)
		CurrentState = EUnitState::MOVED;
	return true;
}

TArray<ATileActor*> AUnitPawn::GetAllTilesInRange()
{
	TArray<ATileActor*> ret;
	TArray<ATileActor*> tilesToCheck;

	tilesToCheck.Add(CurrentTile);
	ATileActor* current = CurrentTile;

	while (tilesToCheck.Num() > 0)
	{
		current = tilesToCheck[0];
		TArray<ATileActor*> adj = current->GetAdjectentTiles();
		for (int i = 0; i < adj.Num(); i++)
		{
			if(!ret.Contains(adj[i]) && !tilesToCheck.Contains(adj[i]))
				if (AMapManager::GetSmallestDistance(CurrentTile, adj[i], true) <= MovementRange)
				{
					if(!adj[i]->Unit || !IsOpposing(adj[i]->Unit))
						tilesToCheck.AddUnique(adj[i]);
				}
		}

		ret.Add(current);
		tilesToCheck.Remove(current);
	}

	return ret;
}

TArray<ATileActor*> AUnitPawn::GetAttackableTiles()
{
	return GetAttackableTiles(CurrentTile);
}

TArray<ATileActor*> AUnitPawn::GetAttackableTiles(ATileActor* tile)
{
	TArray<ATileActor*> ret;
	TArray<ATileActor*> allTiles = tile->GetTilesInRange(MaxAttackRange);
	TArray<ATileActor*> minTiles = tile->GetTilesInRange(MinAttackRange - 1);

	for (int i = 0; i < allTiles.Num(); i++)
	{
		if(!minTiles.Contains(allTiles[i]))
			ret.Add(allTiles[i]);
	}

	return ret;
}

AMapManager* AUnitPawn::GetMap()
{
	if(!UnitOwner)
		return nullptr;

	return UnitOwner->MapData;
}

bool AUnitPawn::IsOpposing(AUnitPawn* target)
{
	if (Affiliation == EUnitSide::PLAYER || Affiliation == EUnitSide::ALLY)
		return target->Affiliation != EUnitSide::PLAYER && target->Affiliation != EUnitSide::ALLY;

	return	Affiliation != target->Affiliation;
}

bool AUnitPawn::IsOpposing()
{
	return 	Affiliation != EUnitSide::PLAYER && Affiliation != EUnitSide::ALLY;
}
