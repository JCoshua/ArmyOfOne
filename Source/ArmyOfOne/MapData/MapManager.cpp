// Fill out your copyright notice in the Description page of Project Settings.


#include "MapManager.h"
#include "MapTileActor.h"
#include "TileAsset.h"
#include <ArmyOfOne/UnitData/UnitActor.h>
#include <ArmyOfOne/ArmyPawns/ArmyPawn.h>

// Sets default values
AMapManager::AMapManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMapManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMapManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMapManager::GenerateMap()
{
	TArray<AMapTileActor*> newMap;

	for (int y = 0; y < MapSizeY; y++)
		for (int x = 0; x < MapSizeX; x++)
		{

			if (GetTile(x, y))
				newMap.Add(GetTile(x, y));
			else
			{
				AMapTileActor* newTile = CreateNewTile();
				newTile->SetActorLocation(GetActorLocation() + FVector(x * 100, y * 100, 0));
				newTile->TileIndexX = x;
				newTile->TileIndexY = y;

				newMap.Add(newTile);
			}
		}

	for (int i = 0; i < GridMap.Num(); i++)
	{
		if (!newMap.Contains(GridMap[i]))
			GridMap[i]->Destroy();
	}
	GridMap = newMap;

	for (int i = 0; i < GridMap.Num(); i++)
	{
		GridMap[i]->GenerateMesh();
		GridMap[i]->MapData = this;
	}
}

AMapTileActor* AMapManager::CreateNewTile()
{
	AMapTileActor* newTile = GetWorld()->SpawnActor<AMapTileActor>();
	return newTile;
}

bool AMapManager::ResetUnitPhase(EUnitPhase units)
{
	if (int(units) >= sides.Num())
		return false;

	AArmyPawn* side = sides[int(units)];

	for (int i = 0; i < side->Units.Num(); i++)
	{
		//side->Units[i]->CurrentState = EUnitState::READY;
		//side->Units[i]->StartTile = side->Units[i]->CurrentTile;
	}

	return false;
}

AMapTileActor* AMapManager::GetTile(int x, int y)
{
	RemoveEmptyGrids();

	for (int i = 0; i < GridMap.Num();i++)
	{
		if (GridMap[i]->TileIndexX == x && GridMap[i]->TileIndexY == y)
			return GridMap[i];
	}
	return nullptr;
}

void AMapManager::RemoveEmptyGrids()
{
	TArray<AMapTileActor*> newArray;

	for (int i = 0; i < GridMap.Num(); i++)
	{
		if (GridMap[i])
			newArray.Add(GridMap[i]);
	}

	GridMap.Empty();
	GridMap = newArray;
}

void AMapManager::SortGrid()
{
	RemoveEmptyGrids();

	int X = GetGreatestHorizontalIndex();
	int Y = GetGreatestVerticalIndex();

	int indexToLower = 0;
	for (int i = 0; i <= X; i++)
	{
		TArray<AMapTileActor*> allInIndex;

		for (int j = 0; j < GridMap.Num(); j++)
		{
			if (GridMap[j]->TileIndexX == i)
				allInIndex.Add(GridMap[j]);
		}

		if (allInIndex.Num() == 0)
			indexToLower++;
		else
		{
			for (int j = 0; j < allInIndex.Num(); j++)
			{
				allInIndex[j]->TileIndexX -= indexToLower;
			}
		}
	}

	indexToLower = 0;
	for (int i = 0; i <= Y; i++)
	{
		TArray<AMapTileActor*> allInIndex;

		for (int j = 0; j < GridMap.Num(); j++)
		{
			if (GridMap[j]->TileIndexY == i)
				allInIndex.Add(GridMap[j]);
		}

		if (allInIndex.Num() == 0)
			indexToLower++;
		else
		{
			for (int j = 0; j < allInIndex.Num(); j++)
			{
				allInIndex[j]->TileIndexY -= indexToLower;
			}
		}
	}
}

int AMapManager::GetGreatestHorizontalIndex()
{
	int X = GridMap[0]->TileIndexX;

	for (int i = 0; i < GridMap.Num(); i++)
	{
		if (GridMap[i]->TileIndexX > X)
			X = GridMap[i]->TileIndexX;
	}

	return X;
}

int AMapManager::GetGreatestVerticalIndex()
{
	int Y = GridMap[0]->TileIndexY;

	for (int i = 0; i < GridMap.Num(); i++)
	{
		if (GridMap[i]->TileIndexY > Y)
			Y = GridMap[i]->TileIndexY;
	}

	return Y;
}

int AMapManager::GetSmallestDistance(AMapTileActor* start, AMapTileActor* end, bool pathForUnit /*=true*/)
{
	if (end->TileData->MovementCost <= 0 || start->MapData != end->MapData)
		return 99;

	//Create an array of nodes based on the Map's grid layout
	TArray<FTileNode*> nodes;
	FTileNode* currentNode;
	for (int i = 0; i < start->MapData->GridMap.Num(); i++)
	{
		nodes.Add(new FTileNode(start->MapData->GridMap[i]));
		if (start->MapData->GridMap[i] == start)
			currentNode = nodes[i];
	}

	//Create two array's of nodes to store pending nodes and store old nodes.
	TArray<FTileNode*> openList;
	TArray<FTileNode*> closedList;

	//Add the first node to the list of pending nodes
	openList.Add(currentNode);

	//While their are still nodes to check
	while (openList.Num() > 0)
	{
		//Find the node with the lowest 'distance' value (The total move cost from the original tile.)
		currentNode = openList[0];
		for (int i = 0; i < openList.Num(); i++)
		{
			if (openList[i]->f < currentNode->f)
				currentNode = openList[i];
		}

		//If the current tile is the goal, return the move cost from the goal.
		if (currentNode->Tile == end)
		{
			return currentNode->g;
		}

		//Make an array of adjenct nodes (for optimization purposes.
		TArray<FTileNode*> adj = currentNode->GetAdjecentNodes(nodes);

		//For each adjecent tile
		for (int i = 0; i < adj.Num(); i++)
		{
			//IF the tile has already been checked, skip the check
			if (closedList.Contains(adj[i]))
				continue;

			//If the tile is not pending, or if the previous move cost was greater than the current cost, skip the check
			if (openList.Contains(adj[i]) && adj[i]->g < currentNode->g + adj[i]->Tile->TileData->MovementCost)
				continue;

			if (pathForUnit)
			{
				if (adj[i]->Tile->Unit /*&& adj[i]->Tile->Unit->IsOpposing(start->Unit)*/)
					continue;
				else if(start->Unit && (!start->Unit->IsAirborne() && !adj[i]->Tile->TileData->IsWalkable))
					continue;
			}
				
			
				//Upates its move cost.
				adj[i]->g = currentNode->g + adj[i]->Tile->TileData->MovementCost;

				//Check how far the tile is from the goal.
				int h = FMath::Abs(adj[i]->Tile->TileIndexX - end->TileIndexX) + FMath::Abs(adj[i]->Tile->TileIndexY - end->TileIndexY);
				adj[i]->f = currentNode->g + adj[i]->Tile->TileData->MovementCost + h;

				//If the tile is not in the open list, add it.
				if (!openList.Contains(adj[i]))
					openList.Add(adj[i]);
		}

		openList.Remove(currentNode);
		closedList.Add(currentNode);
	}

	return 99;
}

TArray<AMapTileActor*> AMapManager::GetShortestPath(AMapTileActor* start, AMapTileActor* end, bool pathForUnit /*=true*/)
{
	if (start->MapData != end->MapData || end->Unit)
		return TArray<AMapTileActor*>();

	//Create an array of nodes based on the Map's grid layout
	TArray<FTileNode*> nodes;
	FTileNode* currentNode;
	FTileNode* startNode = nullptr;
	for (int i = 0; i < start->MapData->GridMap.Num(); i++)
	{
		nodes.Add(new FTileNode(start->MapData->GridMap[i]));
		if (start->MapData->GridMap[i] == start)
		{
			currentNode = nodes[i];
			startNode = nodes[i];
		}
	}

	//Create two array's of nodes to store pending nodes and store old nodes.
	TArray<FTileNode*> openList;
	TArray<FTileNode*> closedList;

	//Add the first node to the list of pending nodes
	openList.Add(currentNode);

	//While their are still nodes to check
	while (openList.Num() > 0)
	{
		//Find the node with the lowest 'distance' value (The total move cost from the original tile.)
		currentNode = openList[0];
		for (int i = 0; i < openList.Num(); i++)
		{
			if (openList[i]->f < currentNode->f)
				currentNode = openList[i];
		}

		//If the current tile is the target, return the path the start node took to get there.
		if (currentNode->Tile == end)
		{
			TArray<AMapTileActor*> path;
			while (currentNode != startNode)
			{
				path.Insert(currentNode->Tile, 0);
				currentNode = currentNode->Previous;
			}
			return path;
		}

		//Make an array of adjenct nodes (for optimization purposes.
		TArray<FTileNode*> adj = currentNode->GetAdjecentNodes(nodes);

		//For each adjecent tile
		for (int i = 0; i < adj.Num(); i++)
		{
			//IF the tile has already been checked, skip the check
			if (closedList.Contains(adj[i]))
				continue;

			//If the tile is not pending, or if the previous move cost was greater than the current cost, skip the check
			if (openList.Contains(adj[i]) && adj[i]->g < currentNode->g + adj[i]->Tile->TileData->MovementCost)
				continue;

			if (pathForUnit)
			{
				if (adj[i]->Tile->Unit /*&& adj[i]->Tile->Unit->IsOpposing(start->Unit)*/)
					continue;
				else if (start->Unit && (!start->Unit->IsAirborne() && !adj[i]->Tile->TileData->IsWalkable))
					continue;
			}


			//Set the adjecent node's previous node to be the currently check node
			adj[i]->Previous = currentNode;
			//Upates its move cost.
			adj[i]->g = currentNode->g + adj[i]->Tile->TileData->MovementCost;

			//Check how far the tile is from the goal.
			int h = FMath::Abs(adj[i]->Tile->TileIndexX - end->TileIndexX) + FMath::Abs(adj[i]->Tile->TileIndexY - end->TileIndexY);
			adj[i]->f = currentNode->g + adj[i]->Tile->TileData->MovementCost + h;

			//If the tile is not in the open list, add it.
			if (!openList.Contains(adj[i]))
				openList.Add(adj[i]);
		}

		openList.Remove(currentNode);
		closedList.Add(currentNode);
	}

	TArray<AMapTileActor*> path;
	while (currentNode != startNode)
	{
		path.Insert(currentNode->Tile, 0);
		currentNode = currentNode->Previous;
	}
	return path;
}

TArray<AMapTileActor*> AMapManager::GetShortestUnscaledPath(AMapTileActor* start, AMapTileActor* end, bool pathForUnit /*=true*/)
{
	if (start->MapData != end->MapData)
		return TArray<AMapTileActor*>();

	//Create an array of nodes based on the Map's grid layout
	TArray<FTileNode*> nodes;
	FTileNode* currentNode;
	FTileNode* startNode = nullptr;
	for (int i = 0; i < start->MapData->GridMap.Num(); i++)
	{
		nodes.Add(new FTileNode(start->MapData->GridMap[i]));
		if (start->MapData->GridMap[i] == start)
		{
			currentNode = nodes[i];
			startNode = nodes[i];
		}
	}

	//Create two array's of nodes to store pending nodes and store old nodes.
	TArray<FTileNode*> openList;
	TArray<FTileNode*> closedList;

	//Add the first node to the list of pending nodes
	openList.Add(currentNode);

	//While their are still nodes to check
	while (openList.Num() > 0)
	{
		//Find the node with the lowest 'distance' value (The total move cost from the original tile.)
		currentNode = openList[0];
		for (int i = 0; i < openList.Num(); i++)
		{
			if (openList[i]->f < currentNode->f)
				currentNode = openList[i];
		}

		//If the current tile is the target, return the path the start node took to get there.
		if (currentNode->Tile == end)
		{
			TArray<AMapTileActor*> path;
			while (currentNode != startNode)
			{
				path.Insert(currentNode->Tile, 0);
				currentNode = currentNode->Previous;
			}
			return path;
		}

		//Make an array of adjenct nodes (for optimization purposes.
		TArray<FTileNode*> adj = currentNode->GetAdjecentNodes(nodes);

		//For each adjecent tile
		for (int i = 0; i < adj.Num(); i++)
		{
			//IF the tile has already been checked, skip the check
			if (closedList.Contains(adj[i]))
				continue;

			if (pathForUnit)
			{
				if (adj[i]->Tile->Unit /*&& adj[i]->Tile->Unit->IsOpposing(start->Unit)*/)
					continue;
				else if (start->Unit && (!start->Unit->IsAirborne() && !adj[i]->Tile->TileData->IsWalkable))
					continue;
			}

			//Set the adjecent node's previous node to be the currently check node
			adj[i]->Previous = currentNode;
			//Upates its move cost.
			adj[i]->g = currentNode->g + 1;

			//Check how far the tile is from the goal.
			int h = FMath::Abs(adj[i]->Tile->TileIndexX - end->TileIndexX) + FMath::Abs(adj[i]->Tile->TileIndexY - end->TileIndexY);
			adj[i]->f = currentNode->g + adj[i]->Tile->TileData->MovementCost + h;

			//If the tile is not in the open list, add it.
			if (!openList.Contains(adj[i]))
				openList.Add(adj[i]);
		}

		openList.Remove(currentNode);
		closedList.Add(currentNode);
	}

	TArray<AMapTileActor*> path;
	while (currentNode != startNode)
	{
		path.Insert(currentNode->Tile, 0);
		currentNode = currentNode->Previous;
	}
	return path;
}

TArray<AMapTileActor*> AMapManager::GetTilesInRange(AMapTileActor* start, int range, bool ignoreUnits /*=true*/)
{
	TArray<AMapTileActor*> ret;
	TArray<AMapTileActor*> tilesToCheck;

	tilesToCheck.Add(start);
	AMapTileActor* current;

	while (tilesToCheck.Num() > 0)
	{
		current = tilesToCheck[0];
		TArray<AMapTileActor*> adj = current->GetAdjectentTiles();
		for (int i = 0; i < adj.Num(); i++)
		{
			if (!ret.Contains(adj[i]) && !tilesToCheck.Contains(adj[i]))
				if (AMapManager::GetSmallestDistance(start, adj[i], !ignoreUnits) <= range)
				{
					if(!adj[i]->Unit)
						tilesToCheck.AddUnique(adj[i]);
				}
		}

		ret.Add(current);
		tilesToCheck.Remove(current);
	}

	return ret;
}

TArray<AMapTileActor*> AMapManager::GetTilesInRange(AUnitActor* start, int range, bool ignoreUnits /*=false*/)
{
	TArray<AMapTileActor*> ret;
	TArray<AMapTileActor*> tilesToCheck;

	tilesToCheck.Add(start->CurrentTile);
	AMapTileActor* current;

	while (tilesToCheck.Num() > 0)
	{
		current = tilesToCheck[0];
		TArray<AMapTileActor*> adj = current->GetAdjectentTiles();
		for (int i = 0; i < adj.Num(); i++)
		{
			if (!ret.Contains(adj[i]) && !tilesToCheck.Contains(adj[i]))
				if (AMapManager::GetSmallestDistance(start->CurrentTile, adj[i], !ignoreUnits) <= range)
				{
					if (!adj[i]->Unit || adj[i]->Unit->Affiliation == start->Affiliation)
						tilesToCheck.AddUnique(adj[i]);
				}
		}

		ret.Add(current);
		tilesToCheck.Remove(current);
	}

	return ret;
}

int AMapManager::GetDistance(AMapTileActor* start, AMapTileActor* end)
{
	return FMath::Abs(start->TileIndexX - end->TileIndexX) + FMath::Abs(start->TileIndexY - end->TileIndexY);
}
