// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileActor.generated.h"

UCLASS()
class ARMYOFONE_API ATileActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATileActor();
	~ATileActor();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* TileMesh;

	//Used to show unit movement.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* MovementMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int VerticalIndex = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int HorizontalIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AMapManager* MapData;

	UPROPERTY(EditAnywhere)
	int MovementCost = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AUnitPawn* Unit;

public:
	TArray<ATileActor*> GetAdjectentTiles(bool includeCorners = false);
	UFUNCTION(BlueprintCallable)
	TArray<ATileActor*> GetTilesInRange(int range);
	
};

struct FTileNode
{
	FTileNode(ATileActor* tile)
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

	ATileActor* Tile;
	int g = 0;
	int h = 0;
	int f = 0;

	FTileNode* Previous;
};