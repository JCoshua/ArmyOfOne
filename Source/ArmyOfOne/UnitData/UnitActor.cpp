// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitActor.h"
#include "ArmyOfOne/MapData/MapTileActor.h"
#include "ArmyOfOne/MapData/MapManager.h"
#include <Components/StaticMeshComponent.h>
#include "ArmyOfOne/ArmyPawns/PlayerArmyPawn.h"
#include "ArmyOfOne/MapData/TileAsset.h"

// Sets default values
AUnitActor::AUnitActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void AUnitActor::BeginPlay()
{
	Super::BeginPlay();
	if (CurrentTile)
		StartTile = CurrentTile;
}

// Called every frame
void AUnitActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentTile)
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
				if (player)
					player->ShouldUpdateRange = true;
			}
		}
	}
}

bool AUnitActor::MoveTile(int xDir, int yDir)
{
	if (!CurrentTile || !CurrentTile->MapData)
		return false;

	int x = CurrentTile->TileIndexX + xDir;
	int y = CurrentTile->TileIndexY + yDir;

	AMapTileActor* tile = GetMap()->GetTile(x, y);

	if (!tile)
		return false;

	if (tile->Unit || tile->TileData->MovementCost <= 0)
		return false;

		CurrentTile->Unit = nullptr;
		CurrentTile = tile;
		tile->Unit = this;

		return true;
}

bool AUnitActor::MoveToTile(int x, int y)
{
	//If the unit is not on a tile, or if the tile is not on a map
	if (!CurrentTile || !CurrentTile->MapData)
		return false;

	//Find the target tile on the map, or return if it doesn't exist
	AMapTileActor* target = GetMap()->GetTile(x, y);
	if (!target)
		return false;

	//Set the unit to move towards the target.
	MovementPath = AMapManager::GetShortestPath(CurrentTile, target);

	//If the unit finished moving (or didn't in this case)...
	if (MovementPath.Num() == 0)
	{
		//Set the unit's State Flag to moved.
		CurrentState = EUnitState::MOVED;

		//If controled by the player, update the visible range of the unit
		APlayerArmyPawn* player = Cast<APlayerArmyPawn>(UnitOwner);
		if (player)
			player->ShouldUpdateRange = true;
	}

	return true;
}

bool AUnitActor::MoveToTile(AMapTileActor* target)
{
	//If the target tile is not on the same map as the unit, return false.
	if (GetMap() != target->MapData)
		return false;

	return MoveToTile(target->TileIndexX, target->TileIndexY);
}

TArray<AMapTileActor*> AUnitActor::GetAllTilesInRange()
{
	return AMapManager::GetTilesInRange(this, MovementRange, false);
}

TArray<AMapTileActor*> AUnitActor::GetAttackableTiles()
{
	return GetAttackableTiles(CurrentTile);
}

TArray<AMapTileActor*> AUnitActor::GetAttackableTiles(AMapTileActor* tile)
{
	TArray<AMapTileActor*> ret;
	TArray<AMapTileActor*> allTiles = tile->GetTilesInRange(1);
	TArray<AMapTileActor*> minTiles = tile->GetTilesInRange(1-1);

	for (int i = 0; i < allTiles.Num(); i++)
	{
		if(!minTiles.Contains(allTiles[i]))
			ret.Add(allTiles[i]);
	}

	return ret;
}

bool AUnitActor::CanAttackUnit(AUnitActor* target)
{
	return GetAttackableTiles().Contains(target->CurrentTile);
}

bool AUnitActor::IsOpposing(AUnitActor* target)
{
	switch (Affiliation)
	{
	case EUnitSide::PLAYER:
		return target->Affiliation != EUnitSide::PLAYER && target->Affiliation != EUnitSide::ALLY;
		break;
	case EUnitSide::ENEMY:
		return target->Affiliation != EUnitSide::ENEMY;
		break;
	case EUnitSide::ALLY:
		return target->Affiliation != EUnitSide::PLAYER && target->Affiliation != EUnitSide::ALLY;
		break;
	case EUnitSide::NEUTRAL:
		return target->Affiliation != EUnitSide::NEUTRAL;
		break;
	}
		
	return	Affiliation != target->Affiliation;
}

bool AUnitActor::IsOpposing()
{
	return Affiliation != EUnitSide::PLAYER && Affiliation != EUnitSide::ALLY;
}

AMapManager* AUnitActor::GetMap()
{
	if(!CurrentTile)
		return nullptr;

	return CurrentTile->MapData;
}

