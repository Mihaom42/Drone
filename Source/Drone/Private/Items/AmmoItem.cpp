// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/AmmoItem.h"
#include "Player/DronePawn.h"
#include "Components/SphereComponent.h"

AAmmoItem::AAmmoItem()
{

}

void AAmmoItem::BeginPlay()
{
	if (SphereComponent->OnComponentBeginOverlap.IsBound())
	{
		SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AAmmoItem::OnOverlap);
	}

	Super::BeginPlay();
}

void AAmmoItem::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ADronePawn* OverlapPawn = Cast<ADronePawn>(OtherActor);

	if (OverlapPawn == nullptr)
	{
		return;
	}

	if (OverlapPawn->PickAmmo(AmmoNumber))
	{
		Super::OnOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	}
}

