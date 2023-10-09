// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseItem.generated.h"

UCLASS()
class DRONE_API ABaseItem : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseItem();

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

protected:
	UPROPERTY(Category = Mesh, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ItemMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* SphereComponent;

	UFUNCTION()
	virtual void OnOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};
