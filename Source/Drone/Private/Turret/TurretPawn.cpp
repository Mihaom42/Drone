// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret/TurretPawn.h"
#include "Perception/PawnSensingComponent.h"
#include "Components/SphereComponent.h"
#include "Player/DronePawn.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Projectile/Projectile.h"
#include "DroneGameState.h"

// Sets default values
ATurretPawn::ATurretPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Base = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base"));

	Leg = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Leg"));
	Leg->SetupAttachment(Base);

	Tower = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tower"));
	Tower->SetupAttachment(Leg);

	Barrel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Barrel"));
	Barrel->SetupAttachment(Tower);

	ProjectileSpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnLocation"));
	ProjectileSpawnLocation->SetupAttachment(Barrel);

	SensingComponentPawn = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("SensingComponent"));
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(Base);
}

// Called when the game starts or when spawned
void ATurretPawn::BeginPlay()
{
	Super::BeginPlay();
	
	SensingComponentPawn->OnSeePawn.AddDynamic(this, &ATurretPawn::OnSeePawn);
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ATurretPawn::OnOverlap);
	OnTurretDestroy.AddDynamic(this, &ATurretPawn::OnTurretDestroyEvent);

	CurrentHealth = MaxHealth;
}

void ATurretPawn::OnSeePawn(APawn* Pawn)
{
	ADronePawn* SeenPawn = Cast<ADronePawn>(Pawn);

	if (SeenPawn == nullptr)
	{
		return;
	}

	FRotator PawnRotator = FMath::RInterpTo(GetActorRotation(),
		UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), SeenPawn->GetActorLocation()), UGameplayStatics::GetWorldDeltaSeconds(SeenPawn), 100.f);
	
	Leg->SetWorldRotation(FRotator(FMath::Clamp(PawnRotator.Pitch, -20.f, 20.f), PawnRotator.Yaw, 0.f));
	SetActorRotation(FRotator(0.f, PawnRotator.Yaw, 0.f));
	
	SpawnProjectile();
}

void ATurretPawn::SpawnProjectile()
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	World->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnLocation->GetComponentTransform());
}

void ATurretPawn::OnTurretDestroyEvent()
{
	UWorld* World = GetWorld();

	if (World == nullptr)
	{
		return;
	}

	ADroneGameState* GameState = Cast<ADroneGameState>(World->GetGameState());

	if (GameState != nullptr)
	{
		GameState->UpdateNumberOfTurrets();
	}
}

void ATurretPawn::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ADronePawn* OverlapPawn = Cast<ADronePawn>(OtherActor);

	if (OverlapPawn == nullptr)
	{
		return;
	}

	FRotator PawnRotator = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), OverlapPawn->GetActorLocation());

	SetActorRotation(FRotator(0.f, PawnRotator.Yaw, 0.f));
}

void ATurretPawn::DamageTurret(int32 DamagePoint)
{
	if (CurrentHealth > 0)
	{
		CurrentHealth -= DamagePoint;
	}

	if (CurrentHealth <= 0)
	{
		OnTurretDestroy.Broadcast();
		Destroy();
	}
}

// Called every frameû
void ATurretPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentHealth <= 0)
	{
		Destroy();
	}
}
