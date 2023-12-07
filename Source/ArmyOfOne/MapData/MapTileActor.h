// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapTileActor.generated.h"

UCLASS()
class ARMYOFONE_API AMapTileActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapTileActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, Category = "Visiblity")
	UStaticMeshComponent* TileMesh;

	UPROPERTY(EditAnywhere, Category = "MapData")
	uint8 TileIndexX;
	UPROPERTY(EditAnywhere, Category = "MapData")
	uint8 TileIndexY;

	UPROPERTY(EditAnywhere, Category = "MapData")
	class UTileAsset* TileData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapData")
	class AUnitActor* Unit;

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "MapData")
	void GenerateMesh();
};
