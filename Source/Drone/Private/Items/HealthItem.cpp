// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/HealthItem.h"
#include "Player/DronePawn.h"
#include "Components/SphereComponent.h"

AHealthItem::AHealthItem()
{

}

void AHealthItem::BeginPlay()
{	
	if (SphereComponent->OnComponentBeginOverlap.IsBound())
	{
		SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AHealthItem::OnOverlap);
	}

	Super::BeginPlay();
}

void AHealthItem::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ADronePawn* OverlapPawn = Cast<ADronePawn>(OtherActor);

	if (OverlapPawn == nullptr)
	{
		return;
	}

	if (OverlapPawn->HealDrone(HealthPointsValue))
	{
		Super::OnOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	}
}


