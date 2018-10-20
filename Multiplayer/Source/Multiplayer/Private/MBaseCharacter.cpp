// Fill out your copyright notice in the Description page of Project Settings.

#include "MBaseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "HealthComponent.h"
#include "GameInstance/MGameInstance.h"
#include "MultiplayerGameModeBase.h"
#include "Announcer/MAnnouncer.h"
#include "Sound/SoundCue.h"
#include "GameFramework/PlayerState.h"
#include "PlayerController/MPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon.h"
#include "Camera/CameraComponent.h"
#include "Net/UnrealNetwork.h"


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

	HealthComp->TakeDamage.AddDynamic(this, &AMBaseCharacter::OnHandleDamage);

	bIsDead = false;

	bWantsToZoom = false;

	bWantsToFire = true;

	ZoomedFOV = 65;

	ZoomInterpSpeed = 20;

}

void AMBaseCharacter::ClientSetPitchRotation()
{
	if (Role < ROLE_Authority)
	{
		ServerSetPitchRotation();
	}

	ControlPitchRotation = GetControlRotation().Pitch;
}

void AMBaseCharacter::ServerSetPitchRotation_Implementation()
{
	ClientSetPitchRotation();
}

bool AMBaseCharacter::ServerSetPitchRotation_Validate()
{
	return true;
}

// Called when the game starts or when spawned
void AMBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	DefaultFOV = GetCamera()->FieldOfView;

	if (Role == ROLE_Authority)
	{
		CurrentWeapon = GetWorld()->SpawnActor<AWeapon>(DefaultWeapon);

		if (CurrentWeapon)
		{
			CurrentWeapon->SetOwner(this);
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "WeaponSocket");
		}
	}
	
}

// Called every frame
void AMBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ZoomHandling();
}


void AMBaseCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	AMultiplayerGameModeBase *GM = Cast<AMultiplayerGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		GM->RespawnDeadPlayer();
	}
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

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMBaseCharacter::StartJump);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMBaseCharacter::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AMBaseCharacter::ExitFire);

	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &AMBaseCharacter::StartAim);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &AMBaseCharacter::EndAim);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMBaseCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMBaseCharacter::EndSprint);
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
	ClientSetPitchRotation();
}

void AMBaseCharacter::TurnRight(float Value)
{
	AddControllerYawInput(Value);
}

void AMBaseCharacter::StartJump()
{
	if (!bWantsToZoom)
	{
		Jump();
	}
}

void AMBaseCharacter::BeginCrouch()
{
	Crouch();
}

void AMBaseCharacter::EndCrouch()
{
	UnCrouch();
}

void AMBaseCharacter::StartAim()
{
	if (bIsSprinting)
	{
		EndSprint();
	}
	bWantsToZoom = true;
	if (CurrentWeapon)
	{
		CurrentWeapon->SetADS(true);
	}
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	if (Role < ROLE_Authority)
	{
		ServerSetMaxWalkSpeed(300.0f);
	}
}

void AMBaseCharacter::EndAim()
{
	bWantsToZoom = false;
	if (CurrentWeapon)
	{
		CurrentWeapon->SetADS(false);
	}
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	if (Role < ROLE_Authority)
	{
		ServerSetMaxWalkSpeed(600.0f);
	}
}

void AMBaseCharacter::StartSprint()
{
	if (!GetCharacterMovement()->IsCrouching() && !bIsSprinting && !bWantsToZoom)
	{
		bIsSprinting = true;
		if (CurrentWeapon)
		{
			ExitFire();
			bWantsToFire = false;
		}
		GetCharacterMovement()->MaxWalkSpeed = 750.0f;
		if (Role < ROLE_Authority)
		{
			ServerSetMaxWalkSpeed(750.0f);
		}
	}
}

void AMBaseCharacter::EndSprint()
{
	if (!GetCharacterMovement()->IsCrouching() && bIsSprinting && !bWantsToZoom)
	{
		bIsSprinting = false;
		if (CurrentWeapon)
		{
			bWantsToFire = true;
		}
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
		if (Role < ROLE_Authority)
		{
			ServerSetMaxWalkSpeed(600.0f);
		}
	}
}

void AMBaseCharacter::StartFire()
{
	if (CurrentWeapon && bWantsToFire)
	{
		CurrentWeapon->StartFire();
	}
}

void AMBaseCharacter::ExitFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->EndFire();
	}
}

void AMBaseCharacter::ServerSetMaxWalkSpeed_Implementation(float Speed)
{
	GetCharacterMovement()->MaxWalkSpeed = Speed;
}

bool AMBaseCharacter::ServerSetMaxWalkSpeed_Validate(float Speed)
{
	return true;
}

void AMBaseCharacter::ZoomHandling()
{
	float TargetFOV = bWantsToZoom ? ZoomedFOV : DefaultFOV;
	float NewFOV = FMath::FInterpTo(GetCamera()->FieldOfView, TargetFOV, GetWorld()->GetDeltaSeconds(),
		ZoomInterpSpeed);

	GetCamera()->SetFieldOfView(NewFOV);
}

void AMBaseCharacter::OnHandleDamage(UHealthComponent *OwningHealthComp,
	AActor* DamagedActor, float Damage, const class UDamageType* DamageType, 
	class AController* InstigatedBy, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Log, TEXT("TAKING DAMAGE"));
	if (HealthComp->GetHealth() <= 0.0f && !bIsDead)
	{
		UMGameInstance *GI = Cast<UMGameInstance>(GetGameInstance());
		if (GI)
		{
			AMPlayerController *PC = Cast<AMPlayerController>(GetController());
			if (PC)
			{
				PC->ClearKillCount();
			}

			PC = Cast<AMPlayerController>(InstigatedBy);
			if (PC)
			{
				PC->AddKill();
			}
		}
		UE_LOG(LogTemp, Warning, TEXT("DEAD"));
		bIsDead = true;
		GetMovementComponent()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		DetachFromControllerPendingDestroy();

		SetLifeSpan(3.0f);
	}
}

void AMBaseCharacter::GetActorEyesViewPoint(FVector& out_Location, FRotator& out_Rotation) const
{
	out_Location = Camera->GetComponentLocation();
	out_Rotation = Camera->GetComponentRotation();
}

void AMBaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMBaseCharacter, CurrentWeapon);
	DOREPLIFETIME(AMBaseCharacter, bIsDead);
	DOREPLIFETIME(AMBaseCharacter, ControlPitchRotation);
	DOREPLIFETIME(AMBaseCharacter, bIsSprinting);
}

