// Fill out your copyright notice in the Description page of Project Settings.


#include "ArmyPawn.h"
#include <ArmyOfOne/UnitPawn.h>

// Sets default values
AArmyPawn::AArmyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AArmyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AArmyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AArmyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool AArmyPawn::AddUnitToSide(AUnitPawn* unitToAdd)
{
	if (!unitToAdd)
		return false;

	Units.Add(unitToAdd);
	unitToAdd->UnitOwner = this;

	return true;
}

bool AArmyPawn::AllUnitsMoved()
{
	for (int i = 0; i < Units.Num(); i++)
	{
		if (Units[i]->CurrentState != EUnitState::WAIT)
			return false;
	}

	return false;
}

bool AArmyPawn::IsOwner(AUnitPawn* unit)
{
	return Units.Contains(unit);
}

