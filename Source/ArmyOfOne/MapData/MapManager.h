// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapManager.generated.h"

class AUnitActor;
class AMapTileActor;
class AArmyPawn;

UENUM(BlueprintType)
enum class EUnitPhase : uint8
{
	PLAYER	UMETA(Displayname = "Player"),
	ENEMY	UMETA(Displayname = "Enemy"),
	ALLY	UMETA(Displayname = "Ally"),
	NEUTRAL UMETA(Displayname = "Neutral"),
};

UCLASS()
class ARMYOFONE_API AMapManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapData")
	uint8 MapSizeX = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapData")
	uint8 MapSizeY = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MapData")
	TArray<AMapTileActor*> GridMap;

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "MapData")
	void GenerateMap();

	AMapTileActor* CreateNewTile();
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AArmyPawn*> sides;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EUnitPhase CurrentPhase;

	bool ResetUnitPhase(EUnitPhase units);
public:

	UFUNCTION(BlueprintCallable)
	AMapTileActor* GetTile(int x, int y);

	UFUNCTION(BlueprintCallable, CallInEditor)
	void RemoveEmptyGrids();

	UFUNCTION(BlueprintCallable, CallInEditor)
	void SortGrid();

	UFUNCTION(BlueprintCallable)
	int GetGreatestHorizontalIndex();

	UFUNCTION(BlueprintCallable)
	int GetGreatestVerticalIndex();

	static int GetSmallestDistance(AMapTileActor* start, AMapTileActor* end, bool pathForUnit = true);

	/// <summary>
	/// Creates the shortest path from the starting tile to the end tile.
	/// </summary>
	/// <param name="start">The starting tile.</param>
	/// <param name="end">The ending tile.</param>
	/// <returns>The route from start to end.</returns>
	static TArray<AMapTileActor*> GetShortestPath(AMapTileActor* start, AMapTileActor* end, bool pathForUnit = true);

	/// <summary>
	/// Creates the shortest path from the starting tile to the end tile, treating all tiles movement cost as 1.
	/// </summary>
	/// <param name="start">The starting tile.</param>
	/// <param name="end">The ending tile.</param>
	/// <returns>The route from start to end.</returns>
	static TArray<AMapTileActor*> GetShortestUnscaledPath(AMapTileActor* start, AMapTileActor* end, bool pathForUnit = true);

public:
	static TArray<AMapTileActor*> GetTilesInRange(AMapTileActor* start, int range, bool ignoreUnits = true);
	static TArray<AMapTileActor*> GetTilesInRange(AUnitActor* start, int range, bool ignoreUnits = false);

	static int GetDistance(AMapTileActor* start, AMapTileActor* end);
};
