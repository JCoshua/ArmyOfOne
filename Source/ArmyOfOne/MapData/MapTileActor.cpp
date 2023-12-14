// Fill out your copyright notice in the Description page of Project Settings.


#include "MapTileActor.h"
#include "MapManager.h"
#include "TileAsset.h"
#include "ArmyOfOne/UnitData/UnitActor.h"

// Sets default values
AMapTileActor::AMapTileActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>("Tile");
	RootComponent = TileMesh;

	MovementMesh = CreateDefaultSubobject<UStaticMeshComponent>("Movement Mesh");
	MovementMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	UStaticMesh* Asset = MeshAsset.Object;
	MovementMesh->SetStaticMesh(Asset);

	if (TileData)
		TileMesh->SetStaticMesh(TileData->Mesh);

	MovementMesh->SetRelativeLocation(FVector(0, 0, 10));
	MovementMesh->SetWorldScale3D(FVector(1, 1, 0.01f));
	MovementMesh->SetVisibility(false);
}

// Called when the game starts or when spawned
void AMapTileActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (Unit)
	{
		Unit->SetActorLocation(GetActorLocation() + FVector(0, 0, 25));
		Unit->CurrentTile = this;
		Unit->StartTile = this;
	}

	MovementMesh->SetRelativeLocation(FVector(0, 0, 10));
	MovementMesh->SetWorldScale3D(FVector(1, 1, 0.01f));
	MovementMesh->SetVisibility(false);
}

// Called every frame
void AMapTileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMapTileActor::GenerateMesh()
{
	if (TileData)
		TileMesh->SetStaticMesh(TileData->Mesh);
}

TArray<AMapTileActor*> AMapTileActor::GetAdjectentTiles(bool includeCorners)
{
	TArray<AMapTileActor*> adj;
	TArray<AMapTileActor*> ret;

	//Add all adjecent tiles
	if(includeCorners)
		adj.Add(MapData->GetTile(TileIndexX + 1, TileIndexY + 1));
	adj.Add(MapData->GetTile(TileIndexX + 1, TileIndexY));
	if (includeCorners)
		adj.Add(MapData->GetTile(TileIndexX - 1, TileIndexY + 1));
	adj.Add(MapData->GetTile(TileIndexX - 1, TileIndexY));
	if (includeCorners)
		adj.Add(MapData->GetTile(TileIndexX + 1, TileIndexY - 1));
	adj.Add(MapData->GetTile(TileIndexX, TileIndexY + 1));
	if (includeCorners)
		adj.Add(MapData->GetTile(TileIndexX - 1, TileIndexY - 1));
	adj.Add(MapData->GetTile(TileIndexX, TileIndexY - 1));

	//Remove any null tiles
	for (int i = 0; i < adj.Num(); i++)
	{
		if (adj[i])
			ret.Add(adj[i]);
	}

	return ret;
}

TArray<AMapTileActor*> AMapTileActor::GetTilesInRange(int range)
{
	TArray<AMapTileActor*> ret;
	TArray<AMapTileActor*> tilesToCheck;

	tilesToCheck.Add(this);
	AMapTileActor* current;

	while (tilesToCheck.Num() > 0)
	{
		current = tilesToCheck[0];
		TArray<AMapTileActor*> adj = current->GetAdjectentTiles();
		for (int i = 0; i < adj.Num(); i++)
		{
			if (ret.Contains(adj[i]) || tilesToCheck.Contains(adj[i]))
				continue;

			if (AMapManager::GetDistance(this, adj[i]) <= range)
				tilesToCheck.Add(adj[i]);
		}

		ret.Add(current);
		tilesToCheck.Remove(current);
	}

	return ret;
}

