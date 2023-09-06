// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemActor.h"
#include "WeaponActor.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	PHYSICAL	UMETA(Displayname = "Physical"),
	MAGIC		UMETA(Displayname = "Magic"),
};

UCLASS()
class ARMYOFONE_API AWeaponActor : public AItemActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int WeaponDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int WeaponAccuracy = 100;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int WeaponCritical;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int WeaponMinRange = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int WeaponMaxRange = 1;

public:
	bool IsPhysicalWeapon() { return WeaponType == EWeaponType::PHYSICAL; }
	virtual int GetDamage() { return WeaponDamage; }
	virtual int GetAccuracy() { return WeaponAccuracy; }
	virtual int GetCritical() { return WeaponCritical; }
};
