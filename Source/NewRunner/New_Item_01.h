// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "New_Item_01.generated.h"

UCLASS()
class NEWRUNNER_API ANew_Item_01 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANew_Item_01();

	// collision component
	UPROPERTY(VisibleAnywhere)
	class USphereComponent* ItemCollisionSphere;

	// mesh component
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* ItemMesh;

	float CollisionSphereRadius;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool BoolFromSweep, const FHitResult& SweepResult);

	static int Score;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
