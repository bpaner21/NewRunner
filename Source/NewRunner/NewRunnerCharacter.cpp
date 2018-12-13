// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "NewRunnerCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "New_Floor_00.h"
#include "New_Item_01.h"

FVector floorPosition;
FVector floorDirection = FVector(1, 0, 0);
int frameCount = 0;

//////////////////////////////////////////////////////////////////////////
// ANewRunnerCharacter

ANewRunnerCharacter::ANewRunnerCharacter()
{
	ANew_Floor_00::FloorCount = 0;
	ANew_Item_01::Score = 0;
	floorDirection = FVector(1, 0, 0);
	floorPosition = FVector(2000, 0, -200);

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	GetCharacterMovement()->MaxWalkSpeed = 800.0F;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetRelativeLocation(FVector(-10.0f, 0.0f, 54.0f));
	SpringArm->SetWorldRotation(FRotator(-20.0f, 0.0f, 0.0f));
	//SpringArm->AttachTo(RootComponent);
	SpringArm->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	SpringArm->TargetArmLength = 525.0f;
	SpringArm->bEnableCameraLag = false;
	SpringArm->bEnableCameraRotationLag = false;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bInheritRoll = true;

	// Create the chase camera component 
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ChaseCamera"));
	//Camera->AttachTo(SpringArm, USpringArmComponent::SocketName);
	Camera->AttachToComponent(SpringArm, FAttachmentTransformRules::SnapToTargetNotIncludingScale, USpringArmComponent::SocketName);
	Camera->SetRelativeRotation(FRotator(10.0f, 0.0f, 0.0f));
	Camera->bUsePawnControlRotation = false;
	Camera->FieldOfView = 90.f;

	direction = 0;
	canTurnRight = true;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void ANewRunnerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	//PlayerInputComponent->BindAxis("MoveForward", this, &ANewRunnerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ANewRunnerCharacter::ChangeDirection);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ANewRunnerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ANewRunnerCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ANewRunnerCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ANewRunnerCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ANewRunnerCharacter::OnResetVR);
}


void ANewRunnerCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ANewRunnerCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ANewRunnerCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ANewRunnerCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ANewRunnerCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ANewRunnerCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ANewRunnerCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ANewRunnerCharacter::ChangeDirection(float Value)
{
	if (Value != 0 && canTurnRight) {
		canTurnRight = false;
		direction += Value;
		if (direction > 3)
			direction = 0;
		else if (direction < 0)
			direction = 3;
	}
	else if (Value == 0)
		canTurnRight = true;
}

void ANewRunnerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (direction == 0)
		MoveForward(1);
	else if (direction == 2)
		MoveForward(-1);
	else if (direction == 1)
		MoveRight(1);
	else if (direction == 3)
		MoveRight(-1);

	ANew_Floor_00 *myFloor;

	ANew_Item_01 *myItem;

	UWorld* const World = GetWorld();
	if (World && (frameCount++ > 15) /*&& (ANew_Floor_00::FloorCount < 20)*/) {
		frameCount = 0;
		FVector SpawnLocation = floorPosition;// GetActorLocation() + FVector(0, newCount * 100, -5);
		floorPosition += floorDirection * 400;
		//updateFloorDirection(floorDirection);
		const FRotator FloorRotation = FRotator(0, 0, 0);
		myFloor = World->SpawnActor<ANew_Floor_00>(SpawnLocation, FloorRotation);

		myItem = World->SpawnActor<ANew_Item_01>(SpawnLocation + FVector(0, 0, 150), FloorRotation);
	}
}