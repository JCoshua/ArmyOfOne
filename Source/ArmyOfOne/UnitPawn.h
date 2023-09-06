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
enum class EUnitSide : uint8
{
	PLAYER	UMETA(Displayname = "Player"),
	ENEMY	UMETA(Displayname = "Enemy"),
	ALLY	UMETA(Displayname = "Ally"),
	NEUTRAL UMETA(Displayname = "Neutral"),
};

UENUM(BlueprintType)
enum class EUnitState : uint8
{
	READY		UMETA(Displayname = "Ready"), //Unit can be moved, but is not yet selected. (Should show range if hovered while no other unit is selected.)
	SELECTED	UMETA(Displayname = "Selected"), //Unit is selected (Should show move range.)
	MOVED		UMETA(Displayname = "Moved"), //Unit has moved to selected destination. (Should only show attack range.)
	WAIT		UMETA(Displayname = "Wait") //Unit has finished action for turn.
};

USTRUCT(BlueprintType)
struct FCombatStats 
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Health = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Strength = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Magic = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Speed = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Skill = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Luck = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Defense = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Resistance = 1;
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

	UFUNCTION(BlueprintCallable)
	bool IsOpposing(AUnitPawn* target);
	bool IsOpposing();


public:
	UPROPERTY(BlueprintReadWrite)
	int CurrentHealth = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCombatStats CombatStats;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MovementRange = 5;

	int GetMHP() { return CombatStats.Health; }
	int GetStr() { return CombatStats.Strength; }
	int GetMag() { return CombatStats.Magic; }
	int GetSpd() { return CombatStats.Speed; }
	int GetSkl() { return CombatStats.Skill; }
	int GetLck() { return CombatStats.Luck; }
	int GetDef() { return CombatStats.Defense; }
	int GetRes() { return CombatStats.Resistance; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AItemActor* EquippedItem;

	TArray<AItemActor*> Inventory;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EUnitSide Affiliation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EUnitState CurrentState;

	TArray<ATileActor*> MovementPath;
public:
	virtual bool IsAirborne() { return false; }
public:
	virtual void OnAttacking(AUnitPawn* defendingUnit);
	virtual void OnDefeated(AUnitPawn* attackingUnit);
	virtual int TakeDamage(AUnitPawn* attackingUnit, int attack, int defense);

private:

};
