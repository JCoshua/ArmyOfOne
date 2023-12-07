// Fill out your copyright notice in the Description page of Project Settings.


#include "MapManager.h"
#include "MapTileActor.h"
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

	for (int x = 0; x < MapSizeX; x++)
		for (int y = 0; y < MapSizeY; y++)
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

int AMapManager::GetSmallestDistance(ATileActor* start, ATileActor* end, bool pathForUnit /*=true*/)
{
	//if (end->MovementCost <= 0 || start->MapData != end->MapData)
		return 99;

	//TArray<FTileNode*> nodes;
	//FTileNode* currentNode;
	//for (int i = 0; i < start->MapData->GridMap.Num(); i++)
	//{
	//	nodes.Add(new FTileNode(start->MapData->GridMap[i]));
	//	if (start->MapData->GridMap[i] == start)
	//		currentNode = nodes[i];
	//}

	//TArray<FTileNode*> openList;
	//TArray<FTileNode*> closedList;
	//openList.Add(currentNode);

	//while (openList.Num() > 0)
	//{
	//	currentNode = openList[0];
	//	for (int i = 0; i < openList.Num(); i++)
	//	{
	//		if (openList[i]->f < currentNode->f)
	//			currentNode = openList[i];
	//	}

	//	if (currentNode->Tile == end)
	//	{
	//		return currentNode->f;
	//	}

	//	TArray<FTileNode*> adj = currentNode->GetAdjecentNodes(nodes);

	//	for (int i = 0; i < adj.Num(); i++)
	//	{
	//		if (pathForUnit && start->Unit && adj[i]->Tile->Unit)
	//		{
	//			//if (!closedList.Contains(adj[i]) && adj[i]->Tile->MovementCost > 0 && !adj[i]->Tile->Unit->IsOpposing(start->Unit))
	//			{
	//				if (!openList.Contains(adj[i]) || (openList.Contains(adj[i]) && adj[i]->g > currentNode->g + adj[i]->Tile->MovementCost))
	//				{
	//					adj[i]->Previous = currentNode;
	//					adj[i]->g = currentNode->g + adj[i]->Tile->MovementCost;

	//					int h = FMath::Abs(adj[i]->Tile->HorizontalIndex - end->HorizontalIndex) + FMath::Abs(adj[i]->Tile->VerticalIndex - end->VerticalIndex);
	//					adj[i]->f = currentNode->g + adj[i]->Tile->MovementCost + h;
	//				}
	//				if (!openList.Contains(adj[i]))
	//					openList.Add(adj[i]);
	//			}
	//		}
	//		else
	//		{
	//			if (!closedList.Contains(adj[i]) && adj[i]->Tile->MovementCost > 0)
	//			{
	//				if (!openList.Contains(adj[i]) || (openList.Contains(adj[i]) && adj[i]->g > currentNode->g + adj[i]->Tile->MovementCost))
	//				{
	//					adj[i]->Previous = currentNode;
	//					adj[i]->g = currentNode->g + adj[i]->Tile->MovementCost;

	//					int h = FMath::Abs(adj[i]->Tile->HorizontalIndex - end->HorizontalIndex) + FMath::Abs(adj[i]->Tile->VerticalIndex - end->VerticalIndex);
	//					adj[i]->f = currentNode->g + adj[i]->Tile->MovementCost + h;
	//				}
	//				if (!openList.Contains(adj[i]))
	//					openList.Add(adj[i]);
	//			}
	//		}
	//	}

	//	openList.Remove(currentNode);
	//	closedList.Add(currentNode);
	//}

	//return 99;
}

TArray<ATileActor*> AMapManager::GetShortestPath(ATileActor* start, ATileActor* end, bool pathForUnit /*=true*/)
{
	//if (start->MapData != end->MapData || end->Unit)
		return TArray<ATileActor*>();

	//TArray<FTileNode*> nodes;
	//FTileNode* currentNode;
	//FTileNode* startNode = nullptr;
	//for (int i = 0; i < start->MapData->GridMap.Num(); i++)
	//{
	//	nodes.Add(new FTileNode(start->MapData->GridMap[i]));
	//	if (start->MapData->GridMap[i] == start)
	//	{
	//		currentNode = nodes[i];
	//		startNode = nodes[i];
	//	}
	//}

	//TArray<FTileNode*> openList;
	//TArray<FTileNode*> closedList;
	//openList.Add(currentNode);

	//while (openList.Num() > 0)
	//{
	//	FTileNode* key = nullptr;
	//	int j = 0;

	//	currentNode = openList[0];
	//	for (int i = 0; i < openList.Num(); i++)
	//	{
	//		if (openList[i]->f < currentNode->f)
	//			currentNode = openList[i];
	//	}


	//	if (currentNode->Tile == end)
	//	{
	//		TArray<ATileActor*> path;
	//		while (currentNode != startNode)
	//		{
	//			path.Insert(currentNode->Tile, 0);
	//			currentNode = currentNode->Previous;
	//		}
	//		return path;
	//	}

	//	TArray<FTileNode*> adj = currentNode->GetAdjecentNodes(nodes);

	//	for (int i = 0; i < adj.Num(); i++)
	//	{
	//		if (pathForUnit && start->Unit && adj[i]->Tile->Unit)
	//		{
	//			//if (!closedList.Contains(adj[i]) && adj[i]->Tile->MovementCost > 0 && !adj[i]->Tile->Unit->IsOpposing(start->Unit))
	//			{
	//				if (!openList.Contains(adj[i]) || (openList.Contains(adj[i]) && adj[i]->g > currentNode->g + adj[i]->Tile->MovementCost))
	//				{
	//					adj[i]->Previous = currentNode;
	//					adj[i]->g = currentNode->g + adj[i]->Tile->MovementCost;

	//					int h = FMath::Abs(adj[i]->Tile->HorizontalIndex - end->HorizontalIndex) + FMath::Abs(adj[i]->Tile->VerticalIndex - end->VerticalIndex);
	//					adj[i]->f = currentNode->g + adj[i]->Tile->MovementCost + h;
	//				}
	//				if (!openList.Contains(adj[i]))
	//					openList.Add(adj[i]);
	//			}
	//		}
	//		else
	//		{
	//			if (!closedList.Contains(adj[i]) && adj[i]->Tile->MovementCost > 0)
	//			{
	//				if (!openList.Contains(adj[i]) || (openList.Contains(adj[i]) && adj[i]->g > currentNode->g + adj[i]->Tile->MovementCost))
	//				{
	//					adj[i]->Previous = currentNode;
	//					adj[i]->g = currentNode->g + adj[i]->Tile->MovementCost;

	//					int h = FMath::Abs(adj[i]->Tile->HorizontalIndex - end->HorizontalIndex) + FMath::Abs(adj[i]->Tile->VerticalIndex - end->VerticalIndex);
	//					adj[i]->f = currentNode->g + adj[i]->Tile->MovementCost + h;
	//				}
	//				if (!openList.Contains(adj[i]))
	//					openList.Add(adj[i]);
	//			}
	//		}
	//	}

	//	openList.Remove(currentNode);
	//	closedList.Add(currentNode);
	//}

	//TArray<ATileActor*> path;
	//while (currentNode != startNode)
	//{
	//	path.Insert(currentNode->Tile, 0);
	//	currentNode = currentNode->Previous;
	//}
	//return path;
}

TArray<ATileActor*> AMapManager::GetShortestUnscaledPath(ATileActor* start, ATileActor* end, bool pathForUnit /*=true*/)
{
	//if (start->MapData != end->MapData)
		return TArray<ATileActor*>();

	//TArray<FTileNode*> nodes;
	//FTileNode* currentNode;
	//FTileNode* startNode = nullptr;
	//for (int i = 0; i < start->MapData->GridMap.Num(); i++)
	//{
	//	nodes.Add(new FTileNode(start->MapData->GridMap[i]));
	//	if (start->MapData->GridMap[i] == start)
	//	{
	//		currentNode = nodes[i];
	//		startNode = nodes[i];
	//	}
	//}

	//TArray<FTileNode*> openList;
	//TArray<FTileNode*> closedList;
	//openList.Add(currentNode);

	//while (openList.Num() > 0)
	//{
	//	currentNode = openList[0];
	//	for (int i = 0; i < openList.Num(); i++)
	//	{
	//		if (openList[i]->f < currentNode->f)
	//			currentNode = openList[i];
	//	}

	//	if (currentNode->Tile == end)
	//	{
	//		TArray<ATileActor*> path;
	//		while (currentNode != startNode)
	//		{
	//			path.Insert(currentNode->Tile, 0);
	//			currentNode = currentNode->Previous;
	//		}
	//		return path;
	//	}

	//	TArray<FTileNode*> adj = currentNode->GetAdjecentNodes(nodes);

	//	for (int i = 0; i < adj.Num(); i++)
	//	{
	//		if (pathForUnit && start->Unit && adj[i]->Tile->Unit)
	//		{
	//			//if (!closedList.Contains(adj[i]) && adj[i]->Tile->MovementCost >= 0 && !adj[i]->Tile->Unit->IsOpposing(start->Unit))
	//			{
	//				if (!openList.Contains(adj[i]) || (openList.Contains(adj[i]) && adj[i]->g > currentNode->g + 1))
	//				{
	//					adj[i]->Previous = currentNode;
	//					adj[i]->g = currentNode->g + 1;

	//					int h = FMath::Abs(adj[i]->Tile->HorizontalIndex - end->HorizontalIndex) + FMath::Abs(adj[i]->Tile->VerticalIndex - end->VerticalIndex);
	//					adj[i]->f = currentNode->g + adj[i]->Tile->MovementCost + h;
	//				}
	//				if (!openList.Contains(adj[i]))
	//					openList.Add(adj[i]);
	//			}
	//		}
	//		else
	//		{
	//			if (!closedList.Contains(adj[i]) && adj[i]->Tile->MovementCost >= 0)
	//			{
	//				if (!openList.Contains(adj[i]) || (openList.Contains(adj[i]) && adj[i]->g > currentNode->g + 1))
	//				{
	//					adj[i]->Previous = currentNode;
	//					adj[i]->g = currentNode->g + 1;

	//					int h = FMath::Abs(adj[i]->Tile->HorizontalIndex - end->HorizontalIndex) + FMath::Abs(adj[i]->Tile->VerticalIndex - end->VerticalIndex);
	//					adj[i]->f = currentNode->g + adj[i]->Tile->MovementCost + h;
	//				}
	//				if (!openList.Contains(adj[i]))
	//					openList.Add(adj[i]);
	//			}
	//		}
	//	}

	//	openList.Remove(currentNode);
	//	closedList.Add(currentNode);
	//}

	//TArray<ATileActor*> path;
	//while (currentNode != startNode)
	//{
	//	path.Insert(currentNode->Tile, 0);
	//	currentNode = currentNode->Previous;
	//}
	//return path;
}