// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnitActor.generated.h"

class AMapTileActor;

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

public:
	//The maximum Hit Points a Unit can have.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxHealth = 1; 

	//Affects the damage the unit inflicts with phyiscal attacks.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Strength;

	//Affects the damage the unit inflicts with magic attacks.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Magic;

	//Affects Avoid. If greater than or equal to a foe's speed+5, unit strikes twice.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Speed;

	//Affects hit rate and frequency of criticals hits.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Skill;

	//Has various effects, including lowering foe's hit chance.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Luck;

	//Reduced the damage of physical attacks.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Defense;

	//Reduced the damage of magical attacks.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Resistance;
};

UCLASS()
class ARMYOFONE_API AUnitActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUnitActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* Mesh;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	FCombatStats CombatStats;
	UPROPERTY(BlueprintReadWrite, Category = "Stats")
	int CurrentHealth = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int MovementRange = 5;

public:
	UFUNCTION(BlueprintCallable, Category="Stats")
	int GetMHP() { return CombatStats.MaxHealth; }
	UFUNCTION(BlueprintCallable, Category="Stats")
	int GetStr() { return CombatStats.Strength; }
	UFUNCTION(BlueprintCallable, Category="Stats")
	int GetMag() { return CombatStats.Magic; }
	UFUNCTION(BlueprintCallable, Category="Stats")
	int GetSpd() { return CombatStats.Speed; }
	UFUNCTION(BlueprintCallable, Category="Stats")
	int GetSkl() { return CombatStats.Skill; }
	UFUNCTION(BlueprintCallable, Category="Stats")
	int GetLck() { return CombatStats.Luck; }
	UFUNCTION(BlueprintCallable, Category="Stats")
	int GetDef() { return CombatStats.Defense; }
	UFUNCTION(BlueprintCallable, Category="Stats")
	int GetRes() { return CombatStats.Resistance; }

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	AMapTileActor* CurrentTile;

	//Used to reset the unit after canceling action, also used to check if unit had moved.
	AMapTileActor* StartTile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EUnitSide Affiliation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	EUnitState CurrentState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	class AArmyPawn* UnitOwner;

	TArray<AMapTileActor*> MovementPath;

public:
	UFUNCTION(BlueprintCallable, Category = "Movement")
	virtual bool MoveTile(int xDir, int yDir);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	virtual bool MoveToTile(int x, int y);
	virtual bool MoveToTile(AMapTileActor* target);
public:
	UFUNCTION(BlueprintCallable, Category = "Movement")
	TArray<AMapTileActor*> GetAllTilesInRange();

	/// <summary>
	/// Gets all tiles within unit's range from current tile.
	/// </summary>
	/// <returns>All tiles the unit can attack.</returns>
	TArray<AMapTileActor*> GetAttackableTiles();

	/// <summary>
	/// Gets all tiles within unit's range from given tile.
	/// </summary>
	/// <param name="tile">The tile to attack from.</param>
	/// <returns>All tiles the unit can attack.</returns>
	UFUNCTION(BlueprintCallable, Category ="MapQuery")
	TArray<AMapTileActor*> GetAttackableTiles(AMapTileActor* tile);

	bool CanAttackUnit(AUnitActor* target);

public:
	bool IsAirborne() { return false; }

	/// <summary>
	/// Check whether this unit opposes the target.
	/// </summary>
	/// <param name="target">The target unit.</param>
	/// <returns>True if this unit and the target units are hostile to each other.</returns>
	UFUNCTION(BlueprintCallable)
	bool IsOpposing(AUnitActor* target);

	/// <summary>
	/// Checks whether this unit opposes the player
	/// </summary>
	/// <returns>True if this unit is not controlled by the player or an allied unit. False otherwise.</returns>
	bool IsOpposing();
private:
	class AMapManager* GetMap();
};
