// Fill out your copyright notice in the Description page of Project Settings.

#include "New_Floor_00.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Engine/CollisionProfile.h"

int ANew_Floor_00::FloorCount = 0;

// Sets default values
ANew_Floor_00::ANew_Floor_00()
{
	++FloorCount;
	SphereRadius = 100.0f;
	BoxExtent = { 20.0f, 20.0f, 20.0f };

	MyCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("My Sphere Component"));
	MyCollisionSphere->InitSphereRadius(SphereRadius);
	MyCollisionSphere->SetCollisionProfileName("Trigger");
	RootComponent = MyCollisionSphere;//*/

	/*MyCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("My Box Component"));
	MyCollisionBox->InitBoxExtent(BoxExtent);
	MyCollisionBox->SetCollisionProfileName("Trigger");
	RootComponent = MyCollisionBox;//*/

	static ConstructorHelpers::FObjectFinder<UStaticMesh> RockMesh(TEXT("/Game/ThirdPerson/Meshes/PlatformStaticMesh.PlatformStaticMesh"));

	// Create the mesh component
	FloorMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ground"));
	//RootComponent = FloorMeshComponent;
	//FloorMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	FloorMeshComponent->SetupAttachment(RootComponent);

	FloorMeshComponent->SetStaticMesh(RockMesh.Object);

	FloorMeshComponent->SetWorldScale3D(FVector(2.0F, 2.0F, 0.2F));

	OnActorEndOverlap.AddDynamic(this, &ANew_Floor_00::OnOverlap);

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANew_Floor_00::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ANew_Floor_00::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANew_Floor_00::OnOverlap(AActor* MyOverlappedActor, AActor* OtherActor)
{
	if (FloorCount > 0)
	{
		--FloorCount;
	}
	else
	{
		FloorCount = 0;
	}//*/

	//--FloorCount;
	//UE_LOG(LogTemp, Log, TEXT("FloorCount: %d"), FloorCount);
	SetLifeSpan(1.0f);
}