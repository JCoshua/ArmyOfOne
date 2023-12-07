// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TileAsset.generated.h"

/**
 * 
 */
UCLASS()
class ARMYOFONE_API UTileAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Visiblity")
	UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, Category = "TileData")
	bool IsWalkable = true;

	UPROPERTY(EditAnywhere, Category = "TileData")
	int MovementCost = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileData")
	int AvoidBonus = 0;
};
