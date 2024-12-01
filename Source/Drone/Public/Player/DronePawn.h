// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Interfaces//OnlineSessionInterface.h"

#include "DronePawn.generated.h"

UCLASS()
class DRONE_API ADronePawn : public APawn
{
	GENERATED_BODY()

public:
	ADronePawn();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	virtual void BeginPlay() override;

private:
	void MoveUp(float Amount);
	void MoveRight(float Amount);
	void OnFire();

protected:
	UFUNCTION(BlueprintCallable)
	void CreateGameSession();

	void OnCreateGameSessionComplete(FName SessionName, bool bWasSuccessful);

public:
	void DamageDrone(int32 DamagePoints);
	bool HealDrone(int32 HealthPoints);
	bool PickAmmo(int32 PickedAmmo);

public:
	/** StaticMesh component that will be the visuals for our flying pawn */
	UPROPERTY(Category = Mesh, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PlaneMesh;

	/** Spring arm that will offset the camera */
	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	/** Camera component that will be our viewpoint */
	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UDroneMovementComponent* MovementComponent;

	//Pointer to the online session interface
	IOnlineSessionPtr OnlineSessionInterface;

private:
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USceneComponent* ProjectileSpawnLocation;

	UPROPERTY(Category = PLane, EditAnywhere)
	float TurnSpeed;

	UPROPERTY(Category = Plane, EditAnywhere)
	float MaxSpeed;

	UPROPERTY(Category = Pitch, EditAnywhere)
	float MinSpeed;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"), Category = Health)
	int32 MaxHealth;
	
	int32 CurrentHealth;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"), Category = Ammo)
	int32 MaxAmmoNumber;

	int32 CurrentAmmoNumber;

	float CurrentForwardSpeed;
	float CurrentYawSpeed;
	float CurrentPitchSpeed;
	float CurrentRollSpeed;

	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;

//
//protected:
//	UFUNCTION(BlueprintCallable)
//	void OpenLobby();
//
//	UFUNCTION(BlueprintCallable)
//	void CallOpenLevel(const FString& Address);
//
//	UFUNCTION(BlueprintCallable)
//	void CallClientTravel(const FString& Address);

};
