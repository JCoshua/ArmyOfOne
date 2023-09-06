// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleManager.h"
#include <ArmyOfOne/UnitPawn.h>
#include <ArmyOfOne/ItemData/WeaponActor.h>
#include <ArmyOfOne/TileActor.h>

void UBattleManager::PerformBattle(AUnitPawn* attackingUnit, AUnitPawn* defendingUnit)
{
    int hit, crit;
    AWeaponActor* attackingWeapon, *defendingWeapon, *activeWeapon;
    AUnitPawn* active, * passive;

    attackingWeapon = dynamic_cast<AWeaponActor*>(attackingUnit->EquippedItem);
    defendingWeapon = dynamic_cast<AWeaponActor*>(defendingUnit->EquippedItem);

    active = attackingUnit;
    passive = defendingUnit;
    activeWeapon = attackingWeapon;

    hit = CalculateHitRate(active, passive, activeWeapon);

    if (hit >= FMath::RandRange(1, 100))
    {
        crit = CalculateCriticalChance(active, passive, activeWeapon);
        passive->CurrentHealth -= CalculateDamage(active, passive, activeWeapon, crit >= FMath::RandRange(1, 100));
    }

    if (!EnsureParticipants(attackingUnit, defendingUnit))
        return;

    if (defendingUnit->CanAttackUnit(attackingUnit))
    {
        active = defendingUnit;
        passive = attackingUnit;
        activeWeapon = defendingWeapon;

        hit = CalculateHitRate(active, passive, activeWeapon);

        if (hit >= FMath::RandRange(1, 100))
        {
            crit = CalculateCriticalChance(active, passive, activeWeapon);
            passive->CurrentHealth -= CalculateDamage(active, passive, activeWeapon, crit >= FMath::RandRange(1, 100));
        }
    }

    if (!EnsureParticipants(attackingUnit, defendingUnit))
        return;

    if (CalculateAttackCount(attackingUnit, defendingUnit, attackingWeapon) == 2)
    {
        active = attackingUnit;
        passive = defendingUnit;
        activeWeapon = attackingWeapon;

        hit = CalculateHitRate(active, passive, activeWeapon);

        if (hit >= FMath::RandRange(1, 100))
        {
            crit = CalculateCriticalChance(active, passive, activeWeapon);
            passive->CurrentHealth -= CalculateDamage(active, passive, activeWeapon, crit >= FMath::RandRange(1, 100));
        }
    }

    if (!EnsureParticipants(attackingUnit, defendingUnit))
        return;

    if (CalculateAttackCount(defendingUnit, attackingUnit, attackingWeapon) == 2 && defendingUnit->CanAttackUnit(attackingUnit))
    {
        active = defendingUnit;
        passive = attackingUnit;
        activeWeapon = defendingWeapon;

        hit = CalculateHitRate(active, passive, activeWeapon);

        if (hit >= FMath::RandRange(1, 100))
        {
            crit = CalculateCriticalChance(active, passive, activeWeapon);
            passive->CurrentHealth -= CalculateDamage(active, passive, activeWeapon, crit >= FMath::RandRange(1, 100));
        }
    }
}

bool UBattleManager::EnsureParticipants(AUnitPawn* attackingUnit, AUnitPawn* defendingUnit)
{
    return attackingUnit->CurrentHealth > 0 && defendingUnit->CurrentHealth > 0;
}

int UBattleManager::GetPower(AUnitPawn* unit, AWeaponActor* weapon)
{
    int pow = 0;

    if (weapon->IsPhysicalWeapon())
        pow = unit->GetStr();
    else
        pow = unit->GetMag();

    return pow + weapon->GetDamage();
}

int UBattleManager::GetHitRate(AUnitPawn* unit, AWeaponActor* weapon)
{
    return (unit->GetSkl() * 3) + weapon->GetAccuracy();
}

int UBattleManager::GetAvoid(AUnitPawn* unit)
{
    int avoid = unit->GetSpd() * 2;

    if (unit->CurrentTile && !unit->IsAirborne())
        avoid += unit->CurrentTile->AvoidBonus;

    return avoid;
}

int UBattleManager::GetCritChance(AUnitPawn* unit, AWeaponActor* weapon)
{
    return unit->GetSkl() + weapon->GetCritical();
}

int UBattleManager::GetCritAvoid(AUnitPawn* unit)
{
    return unit->GetLck();
}

int UBattleManager::GetAgility(AUnitPawn* unit, AWeaponActor* weapon)
{
    return unit->GetSpd();
}

int UBattleManager::CalculateDamage(AUnitPawn* activeUnit, AUnitPawn* passiveUnit, AWeaponActor* weapon, bool isCritical)
{
    int pow, def, damage;

    pow = CalculateAttackPower(activeUnit, passiveUnit, weapon, isCritical);
    def = CalculateDefense(activeUnit, passiveUnit, weapon, isCritical);

    damage = pow - def;

    if (damage <= 0)
        damage = 0;

    if (isCritical)
        damage = FMath::Floor(damage * CritialValue);

    return damage;
}

int UBattleManager::CalculateAttackPower(AUnitPawn* activeUnit, AUnitPawn* passiveUnit, AWeaponActor* weapon, bool isCritical)
{
    int pow = GetPower(activeUnit, weapon);

    if (IsEffective(activeUnit, passiveUnit, weapon))
        pow *= EffectiveValue;

    return pow;
}

int UBattleManager::CalculateDefense(AUnitPawn* activeUnit, AUnitPawn* passiveUnit, AWeaponActor* weapon, bool isCritical)
{
    int def;

    if (weapon->IsPhysicalWeapon())
        def = passiveUnit->GetDef();
    else
        def = passiveUnit->GetRes();

    return def;
}

int UBattleManager::CalculateHitRate(AUnitPawn* activeUnit, AUnitPawn* passiveUnit, AWeaponActor* weapon)
{
    int hit, avoid, percent;

    hit = GetHitRate(activeUnit, weapon);
    avoid = GetAvoid(passiveUnit);

    percent = FMath::Clamp(hit - avoid, 0, 100);

    return percent;
}

int UBattleManager::CalculateCriticalChance(AUnitPawn* activeUnit, AUnitPawn* passiveUnit, AWeaponActor* weapon)
{
    int chance, avoid, percent;

    chance = GetCritChance(activeUnit, weapon);
    avoid = GetCritAvoid(passiveUnit);

    percent = FMath::Clamp(chance - avoid, 0, 100);

    return percent;
}

int UBattleManager::CalculateAttackCount(AUnitPawn* activeUnit, AUnitPawn* passiveUnit, AWeaponActor* weapon)
{
    int active, passive;

    active = GetAgility(activeUnit, weapon);
    passive = GetAgility(passiveUnit, dynamic_cast<AWeaponActor*>(passiveUnit->EquippedItem));

    return (active - passive) >= SpeedDifference ? 2 : 1;
}
