// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DroneMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UDroneMovementComponent::UDroneMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	CurrentForwardSpeed = 100.f;
	TurnSpeed = 70.f;
}


// Called when the game starts
void UDroneMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	//UpdatedComponent = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	SetIsReplicated(true); 
}


// Called every frame
void UDroneMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	LastMove = CreateMove(DeltaTime);

	UnacknowledgedMoves.Add(LastMove);
	Server_SendMove(LastMove);
}

void UDroneMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UDroneMovementComponent, ServerState);
}

void UDroneMovementComponent::MoveToUp(float Amount)
{
	float TargetPitchSpeed = (Amount * TurnSpeed * -1.f);

	TargetPitchSpeed += (FMath::Abs(CurrentYawSpeed) * -0.2f);

	CurrentPitchSpeed = FMath::FInterpTo(CurrentPitchSpeed, TargetPitchSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("PitchSpeed: %f"), CurrentPitchSpeed));
}

void UDroneMovementComponent::MoveToRight(float Amount)
{
	float TargetYawSpeed = (Amount * TurnSpeed);

	CurrentYawSpeed = FMath::FInterpTo(CurrentYawSpeed, TargetYawSpeed, GetWorld()->GetDeltaSeconds(), 2.f);

	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue , FString::Printf(TEXT("YawSpee: %f"), CurrentYawSpeed));

	const bool bIsTurning = FMath::Abs(Amount) > 0.2f;

	float TargetRollSpeed = bIsTurning ? (CurrentYawSpeed * 0.5f) : (GetOwner()->GetActorRotation().Roll * -2.f);

	CurrentRollSpeed = FMath::FInterpTo(CurrentRollSpeed, TargetRollSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("RollSpeed: %f"), CurrentRollSpeed));
}

void UDroneMovementComponent::SimulateMove(FDroneMovement Move)
{
	ApplyRotation(Move);
	UpdateLocationFromVelocity(Move);
}

void UDroneMovementComponent::ApplyRotation(FDroneMovement Move)
{
	FRotator DeltaRotation(0, 0, 0);
	DeltaRotation.Pitch = Move.PitchSpeed * Move.DeltaTime;
	DeltaRotation.Yaw = Move.YawSpeed * Move.DeltaTime;
	DeltaRotation.Roll = Move.RollSpeed * Move.DeltaTime;

	GetOwner()->AddActorLocalRotation(DeltaRotation);
}

void UDroneMovementComponent::UpdateLocationFromVelocity(FDroneMovement Move)
{
	const FVector LocalMove = FVector(CurrentForwardSpeed * Move.DeltaTime, 0.f, 0.f);
	GetOwner()->AddActorLocalOffset(LocalMove, true);}

void UDroneMovementComponent::Server_SendMove_Implementation(FDroneMovement Move)
{
	LastMove = CreateMove(GetWorld()->GetDeltaSeconds());
	SimulateMove(Move);
	UpdateServerState();
}

void UDroneMovementComponent::OnRep_ServerState()
{
	if (GetOwnerRole() == ENetRole::ROLE_AutonomousProxy)
	{
		GetOwner()->SetActorTransform(ServerState.Transform);
		ClearAcknowledgeMoves(ServerState.LastMove);

		for (const FDroneMovement& Move : UnacknowledgedMoves)
		{
			SimulateMove(Move);
		}
	}
}

void UDroneMovementComponent::ClearAcknowledgeMoves(FDroneMovement LastDroneMove)
{
	TArray<FDroneMovement> NewMoves;

	for (const FDroneMovement& Move : UnacknowledgedMoves)
	{
		if (Move.Time > LastDroneMove.Time)
		{
			NewMoves.Add(Move);
		}
	}

	UnacknowledgedMoves = NewMoves;
}

void UDroneMovementComponent::UpdateServerState()
{
	ServerState.LastMove = LastMove;
	ServerState.Transform = GetOwner()->GetTransform();
	ServerState.Velocity = GetOwner()->GetVelocity();
}

FDroneMovement UDroneMovementComponent::CreateMove(float DeltaTime)
{
	FDroneMovement CurrentMove;

	CurrentMove.DeltaTime = DeltaTime;
	CurrentMove.PitchSpeed = CurrentPitchSpeed;
	CurrentMove.RollSpeed = CurrentRollSpeed;
	CurrentMove.YawSpeed = CurrentYawSpeed;
	CurrentMove.Time = GetWorld()->GetTimeSeconds();

	return CurrentMove;
}