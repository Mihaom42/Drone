// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TurretPawn.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTurretDestroy);

class UStaticMeshComponent;

UCLASS()
class DRONE_API ATurretPawn : public APawn
{
	GENERATED_BODY()

public:
	ATurretPawn();

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	UPROPERTY()
	FOnTurretDestroy OnTurretDestroy;

private:	
	UPROPERTY(Category = Mesh, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Base;

	UPROPERTY(Category = Mesh, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Leg;

	UPROPERTY(Category = Mesh, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Tower;

	UPROPERTY(Category = Mesh, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Barrel;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnLocation;

	UPROPERTY(VisibleAnywhere)
	class UPawnSensingComponent* SensingComponentPawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* SphereComponent;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AProjectile> ProjectileClass;

	int32 CurrentHealth;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"), Category = Health)
	int32 MaxHealth;

private:
	UFUNCTION()
	void OnSeePawn(APawn* Pawn);

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void SpawnProjectile(APawn* Pawn);

	UFUNCTION()
	void OnTurretDestroyEvent();

public:
	void DamageTurret(int32 DamagePoint);
};
