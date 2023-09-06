// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArmyPawn.h"
#include "PlayerArmyPawn.generated.h"

class ATileActor;

UCLASS()
class ARMYOFONE_API APlayerArmyPawn : public AArmyPawn
{
	GENERATED_BODY()
	
public:
	// Sets default values for this pawn's properties
	APlayerArmyPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();
	void MoveGridHorizontal(float direction);
	void MoveGridVertical(float direction);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveDelay = 0.1f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Sensitivity = 0.9f;

public:
	void SelectTile();
	void CancelSelection();

public:
	UFUNCTION(BlueprintCallable)
	void ClearMapRange();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ShowMoveRange(AUnitPawn* unit);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ShowAttackRange(AUnitPawn* unit);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	AActor* TracedActor = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	class UStaticMeshComponent* PlayerCursor;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Units)
	AUnitPawn* SelectedUnit;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Units)
	ATileActor* SelectedTile;

	bool ShouldUpdateRange = true;
	UPROPERTY(BlueprintReadWrite)
	bool UsingMouse = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Senitivity = 0.9f;
public:
	virtual void TraceForActor(const FVector& start, const FVector& end, bool DrawDebugHelpers);

	virtual void OnMouseClick();

private:
	float MoveTimer = 0.0f;
};
