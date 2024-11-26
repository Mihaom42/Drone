// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DronePawn.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/World.h"
#include "Engine/StaticMesh.h"
#include "Projectile/Projectile.h"
#include "UI/DroneHUD.h"
#include "Player/DroneMovementComponent.h"

// Sets default values
ADronePawn::ADronePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlaneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaneMesh"));
	RootComponent = PlaneMesh;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 100.0f;
	SpringArm->SocketOffset = FVector(0.f, 0.f, 60.f);
	SpringArm->bEnableCameraLag = false;
	SpringArm->CameraLagSpeed = 15.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;

	ProjectileSpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnLocation"));
	ProjectileSpawnLocation->SetupAttachment(RootComponent);
	ProjectileSpawnLocation->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	
	MovementComponent = CreateDefaultSubobject<UDroneMovementComponent>(TEXT("MovementComponent"));

	TurnSpeed = 70.f;
	CurrentForwardSpeed = 500.f;
}

void ADronePawn::BeginPlay()
{
	CurrentHealth = MaxHealth;
	CurrentAmmoNumber = MaxAmmoNumber;

	ADroneHUD* DroneHUD = Cast<ADroneHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	if (DroneHUD != nullptr)
	{
		DroneHUD->UpdateHealthTextBlockValue(MaxHealth);
		DroneHUD->UpdateAmmoTextBlockValue(MaxAmmoNumber);
	}

	Super::BeginPlay();
}

void ADronePawn::Tick(float DeltaTime)
{
	const FVector LocalMove = FVector(CurrentForwardSpeed * DeltaTime, 0.f, 0.f);

	AddActorLocalOffset(LocalMove, true);

	FRotator DeltaRotation(0, 0, 0);
	DeltaRotation.Pitch = CurrentPitchSpeed * DeltaTime;
	DeltaRotation.Yaw = CurrentYawSpeed * DeltaTime;
	DeltaRotation.Roll = CurrentRollSpeed * DeltaTime;

	AddActorLocalRotation(DeltaRotation);

	Super::Tick(DeltaTime);
}

void ADronePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Moveright", this, &ADronePawn::MoveRight);
	PlayerInputComponent->BindAxis("MoveUp", this, &ADronePawn::MoveUp);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ADronePawn::OnFire);
	//PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
}

void ADronePawn::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	// Deflect along the surface when we collide.
	FRotator CurrentRotation = GetActorRotation();
	SetActorRotation(FQuat::Slerp(CurrentRotation.Quaternion(), HitNormal.ToOrientationQuat(), 0.025f));
}

void ADronePawn::OnFire()
{
	if (CurrentAmmoNumber == 0)
	{
		return;
	}

	if (ProjectileClass == nullptr)
	{
		return;
	}

	UWorld* const World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	const FRotator SpawnRotation = GetActorRotation();
	const FVector SpawnLocation = GetActorLocation() + GetActorRotation().RotateVector(FVector(90.f, 0.f, 0.f));

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	World->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

	CurrentAmmoNumber--;

	ADroneHUD* DroneHUD = Cast<ADroneHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	if (DroneHUD != nullptr)
	{
		DroneHUD->UpdateAmmoTextBlockValue(CurrentAmmoNumber);
	}
}

void ADronePawn::MoveUp(float Amount)
{
	MovementComponent->MoveToUp(Amount);

	/*float TargetPitchSpeed = (Amount * TurnSpeed * -1.f);

	TargetPitchSpeed += (FMath::Abs(CurrentYawSpeed) * -0.2f);

	CurrentPitchSpeed = FMath::FInterpTo(CurrentPitchSpeed, TargetPitchSpeed, GetWorld()->GetDeltaSeconds(), 2.f);*/
}

void ADronePawn::MoveRight(float Amount)
{
	MovementComponent->MoveToRight(Amount);

	/*float TargetYawSpeed = (Amount * TurnSpeed);

	//CurrentYawSpeed = FMath::FInterpTo(CurrentYawSpeed, TargetYawSpeed, GetWorld()->GetDeltaSeconds(), 2.f);

	//const bool bIsTurning = FMath::Abs(Amount) > 0.2f;

	//float TargetRollSpeed = bIsTurning ? (CurrentYawSpeed * 0.5f) : (GetActorRotation().Roll * -2.f);

	CurrentRollSpeed = FMath::FInterpTo(CurrentRollSpeed, TargetRollSpeed, GetWorld()->GetDeltaSeconds(), 2.f);*/
}

void ADronePawn::DamageDrone(int32 DamagePoints)
{
	ADroneHUD* DroneHUD = Cast<ADroneHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	if (CurrentHealth > 0)
	{
		CurrentHealth -= DamagePoints;
	}

	if (CurrentHealth <= 0)
	{
		Destroy();

		if (DroneHUD != nullptr)
		{
			DroneHUD->ShowGameOverWidget();
		}

		return;
	}

	if (DroneHUD != nullptr)
	{
		DroneHUD->UpdateHealthTextBlockValue(CurrentHealth);
	}
}

bool ADronePawn::HealDrone(int32 HealthPoints)
{
	if (CurrentHealth == MaxHealth)
	{
		return false;
	}

	CurrentHealth += HealthPoints;
	
	if (CurrentHealth > MaxHealth)
	{
		CurrentHealth = MaxHealth;
	}

	ADroneHUD* DroneHUD = Cast<ADroneHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	if (DroneHUD != nullptr)
	{
		DroneHUD->UpdateHealthTextBlockValue(CurrentHealth);
	}

	return true;
}

bool ADronePawn::PickAmmo(int32 PickedAmmo)
{
	if (CurrentAmmoNumber == MaxAmmoNumber)
	{
		return false;
	}

	CurrentAmmoNumber += PickedAmmo;

	if (CurrentAmmoNumber > MaxAmmoNumber)
	{
		CurrentAmmoNumber = MaxAmmoNumber;
	}

	ADroneHUD* DroneHUD = Cast<ADroneHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	if (DroneHUD != nullptr)
	{
		DroneHUD->UpdateAmmoTextBlockValue(CurrentAmmoNumber);
	}

	return true;
}

