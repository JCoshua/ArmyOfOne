// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Engine/DataTable.h"
#include "UnitPawn.generated.h"

class ATileActor;
class AArmyPawn;
class AItemActor;

UENUM(BlueprintType)
enum class EPawnSide : uint8
{
	PLAYER	UMETA(Displayname = "Player"),
	ENEMY	UMETA(Displayname = "Enemy"),
	ALLY	UMETA(Displayname = "Ally"),
	NEUTRAL UMETA(Displayname = "Neutral"),
};

UENUM(BlueprintType)
enum class EPawnState : uint8
{
	READY		UMETA(Displayname = "Ready"), //Unit can be moved, but is not yet selected. (Should show range if hovered while no other unit is selected.)
	SELECTED	UMETA(Displayname = "Selected"), //Unit is selected (Should show move range.)
	MOVED		UMETA(Displayname = "Moved"), //Unit has moved to selected destination. (Should only show attack range.)
	WAIT		UMETA(Displayname = "Wait") //Unit has finished action for turn.
};

UCLASS()
class ARMYOFONE_API AUnitPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AUnitPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AArmyPawn* UnitOwner;

	UFUNCTION(BlueprintCallable)
	class AMapManager* GetMap();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ATileActor* CurrentTile;

	//Used to reset the unit after canceling action, also used to check if unit had moved.
	ATileActor* StartTile;

	UFUNCTION(BlueprintCallable)
	virtual bool MoveTile(int xDir, int yDir);

	UFUNCTION(BlueprintCallable)
	virtual bool MoveToTile(int x, int y);
	virtual bool MoveToTile(ATileActor* target);

	UFUNCTION(BlueprintCallable)
	TArray<ATileActor*> GetAllTilesInRange();

	/// <summary>
	/// Gets all tiles within unit's range from current tile.
	/// </summary>
	/// <returns>All tiles the unit can attack.</returns>
	TArray<ATileActor*> GetAttackableTiles();

	/// <summary>
	/// Gets all tiles within unit's range from given tile.
	/// </summary>
	/// <param name="tile">The tile to attack from.</param>
	/// <returns>All tiles the unit can attack.</returns>
	UFUNCTION(BlueprintCallable)
	TArray<ATileActor*> GetAttackableTiles(ATileActor* tile);
	bool CanAttackUnit(AUnitPawn* target);

	/*UFUNCTION(BlueprintCallable)
	bool IsOpposing(AUnitPawn* target);
	bool IsOpposing();*/


public:
	UPROPERTY(BlueprintReadWrite)
	int CurrentHealth = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MovementRange = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AItemActor* EquippedItem;

	TArray<AItemActor*> Inventory;
	
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPawnState CurrentState;

	TArray<ATileActor*> MovementPath;
public:
	virtual bool IsAirborne() { return false; }
public:
	virtual void OnAttacking(AUnitPawn* defendingUnit);
	virtual void OnDefeated(AUnitPawn* attackingUnit);

private:

};
