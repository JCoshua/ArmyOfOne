// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ArmyPawn.generated.h"

class AUnitPawn;

UCLASS()
class ARMYOFONE_API AArmyPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AArmyPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Units)
	TArray<AUnitPawn*> Units;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Units)
	class AMapManager* MapData;

public:
	virtual bool AddUnitToSide(AUnitPawn* unitToAdd);

	virtual bool AllUnitsMoved();

	virtual bool IsOwner(AUnitPawn* unit);
};
