// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"
#include "Multiplayer.h"
#include "MBaseCharacter.h"
#include "Components/SkeletalMeshComponent.h"


// Sets default values
AWeapon::AWeapon()
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

}

void AWeapon::StartFire()
{
	if (MyOwner)
	{
		Fire();
	}
}

void AWeapon::EndFire()
{
	if (MyOwner)
	{
		EndFire();
	}
}

void AWeapon::Fire()
{
	if (MyOwner)
	{
		FVector EyeLocation;
		FRotator EyeRotation;
		FVector TraceEnd;

		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		TraceEnd = EyeLocation + (EyeRotation.Vector() * 10000);

		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(MyOwner);
		CollisionParams.AddIgnoredActor(this);
		CollisionParams.bTraceComplex = true;

		FHitResult Hit;

		if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, COLLISION_WEAPON, CollisionParams))
		{
			// DO STUFF
		}
	}
}


void AWeapon::SetCurrentOwner(AMBaseCharacter *NewOwner)
{
	if (!MyOwner)
	{
		MyOwner = NewOwner;
	}
}
