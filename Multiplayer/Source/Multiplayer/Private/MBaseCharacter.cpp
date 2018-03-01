// Fill out your copyright notice in the Description page of Project Settings.

#include "MBaseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "HealthComponent.h"
#include "Camera/CameraComponent.h"


// Sets default values
AMBaseCharacter::AMBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArmComp);

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

}

// Called when the game starts or when spawned
void AMBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMBaseCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector() * Value);
}

void AMBaseCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector() * Value);
}

void AMBaseCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void AMBaseCharacter::TurnRight(float Value)
{
	AddControllerYawInput(Value);
}

void AMBaseCharacter::BeginCrouch()
{
	Crouch();
}

void AMBaseCharacter::EndCrouch()
{
	UnCrouch();
}

// Called every frame
void AMBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMBaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMBaseCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &AMBaseCharacter::LookUp);
	PlayerInputComponent->BindAxis("TurnRight", this, &AMBaseCharacter::TurnRight);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AMBaseCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AMBaseCharacter::EndCrouch);
}

