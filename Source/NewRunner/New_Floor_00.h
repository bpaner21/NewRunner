// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "New_Floor_00.generated.h"

UCLASS()
class NEWRUNNER_API ANew_Floor_00 : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Enemy, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* FloorMeshComponent;
	
public:	
	static int FloorCount;

	// Sets default values for this actor's properties
	ANew_Floor_00();

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* MyCollisionSphere;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* MyCollisionBox;

	float SphereRadius;
	FVector BoxExtent;

	//UFUNCTION()
	//void EndOverlap(AActor* MyOverlappedActor, AActor* OtherActor);
	//void EndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnOverlap(AActor* MyOverlappedActor, AActor* OtherActor);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
