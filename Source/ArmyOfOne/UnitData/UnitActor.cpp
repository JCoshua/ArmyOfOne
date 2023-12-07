// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitActor.h"
#include "ArmyOfOne/TileActor.h"
#include <Components/StaticMeshComponent.h>

// Sets default values
AUnitActor::AUnitActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void AUnitActor::BeginPlay()
{
	Super::BeginPlay();
	/*if (CurrentTile)
	{
		StartTile = CurrentTile;
		CurrentTile->Unit = this;
	}*/
		
}

// Called every frame
void AUnitActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

