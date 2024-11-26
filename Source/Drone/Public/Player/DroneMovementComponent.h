// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DroneMovementComponent.generated.h"

USTRUCT()
struct FDroneMovement
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	float YawSpeed;
	
	UPROPERTY()
	float PitchSpeed;
	
	UPROPERTY()
	float RollSpeed;
	
	UPROPERTY()
	float DeltaTime;
	
	UPROPERTY()
	float Time;
};

USTRUCT()
struct FDroneState
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	FTransform Transform;
	
	UPROPERTY()
	FVector Velocity;
	
	UPROPERTY()
	FDroneMovement LastMove;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRONE_API UDroneMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UDroneMovementComponent();
	
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	
	UPROPERTY(Category = Plane, EditAnywhere)
	float TurnSpeed;

	float CurrentForwardSpeed;
	float CurrentYawSpeed;
	float CurrentPitchSpeed;
	float CurrentRollSpeed;

	float UpDirection;
	float RightDirection;
	
	FDroneMovement LastMove;

	TArray<FDroneMovement> UnacknowledgedMoves;

	UPROPERTY(ReplicatedUsing = OnRep_ServerState)
	FDroneState ServerState;

	UFUNCTION()
	void OnRep_ServerState();

	UFUNCTION(Server, Reliable)
	void Server_SendMove(FDroneMovement Move);

	void SimulateMove(FDroneMovement Move);
	void ApplyRotation(FDroneMovement Move);
	void UpdateLocationFromVelocity(FDroneMovement Move);
	void UpdateServerState();
	FDroneMovement CreateMove(float DeltaTime);
	void ClearAcknowledgeMoves(FDroneMovement LastDroneMove);

public:
	void MoveToUp(float Amount);
	void MoveToRight(float Amount);

	FDroneMovement GetLastMove() { return LastMove; }

	void SetYawSpeed(float Value) { CurrentYawSpeed = Value; }
	void SetPitchSpeed(float Value) { CurrentPitchSpeed = Value; }
	void SetRollSpeed(float Value) { CurrentRollSpeed = Value; }
};
