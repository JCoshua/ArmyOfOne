// Fill out your copyright notice in the Description page of Project Settings.


#include "TileActor.h"
#include "MapData/MapManager.h"
#include <Components/StaticMeshComponent.h>
#include "UnitData/UnitActor.h"

// Sets default values
ATileActor::ATileActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>("Tile");
	RootComponent = TileMesh;

	MovementMesh = CreateDefaultSubobject<UStaticMeshComponent>("Movement Mesh");
	MovementMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATileActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TArray<ATileActor*> ATileActor::GetAdjectentTiles(bool includeCorners /*= false*/)
{
	TArray<ATileActor*> adj;
	TArray<ATileActor*> ret;

	//Add all adjecent tiles
	//if(includeCorners)
	//	adj.Add(MapData->GetTile(HorizontalIndex + 1, VerticalIndex + 1));
	//adj.Add(MapData->GetTile(HorizontalIndex + 1, VerticalIndex));
	//if (includeCorners)
	//	adj.Add(MapData->GetTile(HorizontalIndex - 1, VerticalIndex + 1));
	//adj.Add(MapData->GetTile(HorizontalIndex - 1, VerticalIndex));
	//if (includeCorners)
	//	adj.Add(MapData->GetTile(HorizontalIndex + 1, VerticalIndex - 1));
	//adj.Add(MapData->GetTile(HorizontalIndex, VerticalIndex + 1));
	//if (includeCorners)
	//	adj.Add(MapData->GetTile(HorizontalIndex - 1, VerticalIndex - 1));
	//adj.Add(MapData->GetTile(HorizontalIndex, VerticalIndex - 1));

	//Remove any null tiles
	for (int i = 0; i < adj.Num(); i++)
	{
		if (adj[i])
			ret.Add(adj[i]);
	}

	return ret;
}

TArray<ATileActor*> ATileActor::GetTilesInRange(int range)
{
	if (range < 1)
		return TArray<ATileActor*>();

	TArray<ATileActor*> ret;

	/*TArray<ATileActor*> grid = MapData->GridMap;
	for (int i = 0; i < grid.Num(); i++)
	{
		int xDif = FMath::Abs(grid[i]->HorizontalIndex - HorizontalIndex);
		int yDif = FMath::Abs(grid[i]->VerticalIndex - VerticalIndex);

		if (xDif + yDif <= range)
			ret.Add(grid[i]);
	}*/

	return ret;
}



