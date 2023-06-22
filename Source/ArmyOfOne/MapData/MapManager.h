// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapManager.generated.h"

class ATileActor;
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AArmyPawn*> sides;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EUnitPhase CurrentPhase;

	bool ResetUnitPhase(EUnitPhase units);
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<ATileActor*> GridMap;

	UFUNCTION(BlueprintCallable)
	void AddTileToGrid(ATileActor* newTile);
	/// <summary>
	/// Normalizes the grid so the starting index is 0,0.
	/// </summary>
	void NormalizeGridIndex();

	UFUNCTION(BlueprintCallable)
	ATileActor* GetTile(int x, int y);

	UFUNCTION(BlueprintCallable, CallInEditor)
	void RemoveEmptyGrids();

	UFUNCTION(BlueprintCallable, CallInEditor)
	void SortGrid();

	UFUNCTION(BlueprintCallable)
	int GetGreatestHorizontalIndex();

	UFUNCTION(BlueprintCallable)
	int GetGreatestVerticalIndex();

	static int GetSmallestDistance(ATileActor* start, ATileActor* end, bool pathForUnit = true);

	/// <summary>
	/// Creates the shortest path from the starting tile to the end tile.
	/// </summary>
	/// <param name="start">The starting tile.</param>
	/// <param name="end">The ending tile.</param>
	/// <returns>The route from start to end.</returns>
	static TArray<ATileActor*> GetShortestPath(ATileActor* start, ATileActor* end, bool pathForUnit = true);

	/// <summary>
	/// Creates the shortest path from the starting tile to the end tile, treating all tiles movement cost as 1.
	/// </summary>
	/// <param name="start">The starting tile.</param>
	/// <param name="end">The ending tile.</param>
	/// <returns>The route from start to end.</returns>
	static TArray<ATileActor*> GetShortestUnscaledPath(ATileActor* start, ATileActor* end, bool pathForUnit = true);
};
