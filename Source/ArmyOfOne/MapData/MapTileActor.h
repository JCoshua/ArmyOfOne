// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapTileActor.generated.h"

UCLASS()
class ARMYOFONE_API AMapTileActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapTileActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, Category = "Visiblity")
	UStaticMeshComponent* TileMesh;

	//Used to show unit movement.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visiblity")
	UStaticMeshComponent* MovementMesh;

public:
	UPROPERTY(EditAnywhere, Category = "MapData")
	uint8 TileIndexX;
	UPROPERTY(EditAnywhere, Category = "MapData")
	uint8 TileIndexY;
	UPROPERTY(EditAnywhere, Category = "MapData")
	class AMapManager* MapData;

	UPROPERTY(EditAnywhere, Category = "MapData")
	class UTileAsset* TileData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapData")
	class AUnitActor* Unit;

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "MapData")
	void GenerateMesh();

public:
	TArray<AMapTileActor*> GetAdjectentTiles(bool includeCorners = false);
	UFUNCTION(BlueprintCallable)
	TArray<AMapTileActor*> GetTilesInRange(int range);
};

struct FTileNode
{
	FTileNode(AMapTileActor* tile)
	{
		Tile = tile;
	}

	/// <summary>
	/// Given a list of nodes, returns all that are adjacent to this node.
	/// </summary>
	/// <param name="nodeList">The list of nodes.</param>
	/// <returns>All adjacent nodes from the list.</returns>
	TArray<FTileNode*> GetAdjecentNodes(TArray<FTileNode*> nodeList)
	{
		TArray<FTileNode*> ret;
		for (int i = 0; i < nodeList.Num(); i++)
		{
			if (Tile->GetAdjectentTiles().Contains(nodeList[i]->Tile))
				ret.Add(nodeList[i]);

		}
		return ret;
	}

	AMapTileActor* Tile;
	int g = 0;
	int h = 0;
	int f = 0;

	FTileNode* Previous;
};