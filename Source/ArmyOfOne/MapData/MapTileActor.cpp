// Fill out your copyright notice in the Description page of Project Settings.


#include "MapTileActor.h"
#include "TileAsset.h"
#include "ArmyOfOne/UnitData/UnitActor.h"

// Sets default values
AMapTileActor::AMapTileActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>("Tile");
	RootComponent = TileMesh;

	if (TileData)
		TileMesh->SetStaticMesh(TileData->Mesh);
}

// Called when the game starts or when spawned
void AMapTileActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (Unit)
		Unit->SetActorLocation(GetActorLocation() + FVector(0, 0, 25));
}

// Called every frame
void AMapTileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMapTileActor::GenerateMesh()
{
	if (TileData)
		TileMesh->SetStaticMesh(TileData->Mesh);
}

