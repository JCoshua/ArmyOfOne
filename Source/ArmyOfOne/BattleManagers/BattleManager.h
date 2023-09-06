// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BattleManager.generated.h"

class AUnitPawn;
class AWeaponActor;
UCLASS()
class ARMYOFONE_API UBattleManager : public UObject
{
	GENERATED_BODY()
	
public:
	static constexpr float EffectiveValue = 3.0f;
	static constexpr float CritialValue = 3.0f;
	static constexpr int SpeedDifference = 5;
public:
	static void PerformBattle(AUnitPawn* attackingUnit, AUnitPawn* defendingUnit);
	static bool EnsureParticipants(AUnitPawn* attackingUnit, AUnitPawn* defendingUnit);
public:
	static int GetPower(AUnitPawn* unit, AWeaponActor* weapon);
	static int GetHitRate(AUnitPawn* unit, AWeaponActor* weapon);
	static int GetAvoid(AUnitPawn* unit);
	static int GetCritChance(AUnitPawn* unit, AWeaponActor* weapon);
	static int GetCritAvoid(AUnitPawn* unit);
	static int GetAgility(AUnitPawn* unit, AWeaponActor* weapon);

public:
	static int CalculateDamage(AUnitPawn* activeUnit, AUnitPawn* passiveUnit, AWeaponActor* weapon, bool isCritical);
	static int CalculateAttackPower(AUnitPawn* activeUnit, AUnitPawn* passiveUnit, AWeaponActor* weapon, bool isCritical);
	static int CalculateDefense(AUnitPawn* activeUnit, AUnitPawn* passiveUnit, AWeaponActor* weapon, bool isCritical);
	static bool IsEffective(AUnitPawn* activeUnit, AUnitPawn* passiveUnit, AWeaponActor* weapon) { return false; };

public:
	static int CalculateHitRate(AUnitPawn* activeUnit, AUnitPawn* passiveUnit, AWeaponActor* weapon);
	static int CalculateCriticalChance(AUnitPawn* activeUnit, AUnitPawn* passiveUnit, AWeaponActor* weapon);

	static int CalculateAttackCount(AUnitPawn* activeUnit, AUnitPawn* passiveUnit, AWeaponActor* weapon);
};
