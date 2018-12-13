// Fill out your copyright notice in the Description page of Project Settings.

#include "New_Item_01.h"
#include "DrawDebugHelpers.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"

// void OnOverlapBegin(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool BoolFromSweep, const FHitResult& SweepResult)

int ANew_Item_01::Score = 0;

// Sets default values
ANew_Item_01::ANew_Item_01()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionSphereRadius = 25.0f;

	ItemCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Item Sphere Component"));
	ItemCollisionSphere->InitSphereRadius(CollisionSphereRadius);
	ItemCollisionSphere->SetCollisionProfileName("Trigger");

	static ConstructorHelpers::FObjectFinder<UStaticMesh> RockMesh(TEXT("/Game/ThirdPerson/Meshes/PlatformStaticMesh.PlatformStaticMesh"));

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item Mesh"));

	ItemMesh->SetStaticMesh(RockMesh.Object);
	ItemMesh->SetWorldScale3D(FVector(0.5f, 0.5f, 0.5f));

	ItemMesh->SetupAttachment(RootComponent);

	ItemCollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ANew_Item_01::OnOverlapBegin);

}

// Called when the game starts or when spawned
void ANew_Item_01::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANew_Item_01::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DrawDebugSphere(GetWorld(), GetActorLocation(), CollisionSphereRadius, 20, FColor::Purple, false, -1, 0, 1);
}

void ANew_Item_01::OnOverlapBegin(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool BoolFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComponent != nullptr))
	{
		//SetLifeSpan(1.0f);
		++Score;
		UE_LOG(LogTemp, Log, TEXT("Score: %d"), Score);
		GEngine->AddOnScreenDebugMessage(0, 10.0f, FColor::Blue, FString(TEXT("Score: " + FString::FromInt(Score))));
		Destroy();
	}
}