// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"
#include "Multiplayer.h"
#include "MBaseCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "DisplayDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "ParticleDefinitions.h"
#include "Particles/ParticleSystemComponent.h"
#include "Camera/CameraShake.h"
#include "Animation/AnimationAsset.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "DrawDebugHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AWeapon::AWeapon()
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	bIsADS = false;

	SetReplicates(true);

	NetUpdateFrequency = 66.0f;
	MinNetUpdateFrequency = 33.0f;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	TimeBetweenShots = 60 / WeaponConfig.RateOfFire;
}

void AWeapon::StartFire()
{
	GetWorldTimerManager().SetTimer(TimeBetweenShotsHandle, this, &AWeapon::Fire,
		TimeBetweenShots, true, 0.0f);
}

void AWeapon::EndFire()
{
	GetWorldTimerManager().ClearTimer(TimeBetweenShotsHandle);
}

void AWeapon::Fire()
{
	if (Role < ROLE_Authority)
	{
		ServerFire();
	}

	AActor *MyOwner = GetOwner();

	if (MyOwner)
	{
		FVector EyeLocation;
		FRotator EyeRotation;
		FVector TraceEnd;

		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector ShotDirection = EyeRotation.Vector();

		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(MyOwner);
		CollisionParams.AddIgnoredActor(this);
		CollisionParams.bTraceComplex = true;
		CollisionParams.bReturnPhysicalMaterial = true;
		PlayWeaponAnimation(WeaponConfig.FireAnimation, false);
		UParticleSystemComponent *SpawnedFX = UGameplayStatics::SpawnEmitterAttached(WeaponConfig.MuzzleFX,
			GetMesh(), "Muzzle");
		PlayWeaponRecoil(WeaponConfig.WeaponRecoil);
		float WeaponSpread = bIsADS ? FMath::DegreesToRadians(WeaponConfig.AimWeaponSpread) :
			FMath::DegreesToRadians(WeaponConfig.HipWeaponSpread);
		FVector SpreadVector = FMath::VRandCone(ShotDirection, WeaponSpread);

		TraceEnd = EyeLocation + (SpreadVector * 10000);

		FHitResult Hit;

		TArray<FHitResult> Hits;

		if (GetWorld()->LineTraceMultiByChannel(Hits, EyeLocation, TraceEnd, COLLISION_WEAPON, CollisionParams))
		{
			for (int i = 0; i < Hits.Num(); i++)
			{
				FVector SocketLoc = MyOwner->GetActorLocation() - Hits[i].Location;
				FVector HitLoc = SpreadVector * -1;

				float HitDot = FVector::DotProduct(SocketLoc.GetSafeNormal(), HitLoc);
				if (HitDot > 0.5f)
				{
					AActor *LineHitActor = Hits[i].GetActor();

					if (LineHitActor && DamageType)
					{
						EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());

						float Damage = 25.0f;
						switch (SurfaceType)
						{
						case SURFACE_DAMAGEDEFAULT:
							Damage = 10.0f;
							break;
						case SURFACE_DAMAGEHEAD:
							Damage = 80.0f;
							break;
						default:
							break;
						}

						TraceEnd = Hits[i].ImpactPoint;
						UGameplayStatics::ApplyPointDamage(LineHitActor, Damage, ShotDirection,
							Hits[i], MyOwner->GetInstigatorController(), this, DamageType);

						PlayImpactFX(TraceEnd);
					}
				}
			}
		}

		/*
		if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, COLLISION_WEAPON, CollisionParams))
		{
			AActor *LineHitActor = Hit.GetActor();

			if (LineHitActor && DamageType)
			{
				EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());

				float Damage = 25.0f;
				switch (SurfaceType)
				{
				case SURFACE_DAMAGEDEFAULT:
					Damage = 10.0f;
					break;
				case SURFACE_DAMAGEHEAD:
					Damage = 80.0f;
					break;
				default:
					break;
				}

				TraceEnd = Hit.ImpactPoint;
				UGameplayStatics::ApplyPointDamage(LineHitActor, Damage, ShotDirection,
					Hit, MyOwner->GetInstigatorController(), this, DamageType);
			}
		}
		*/
		PlayTracerFX(TraceEnd);

		if (Role == ROLE_Authority)
		{
			HitScanTrace.TraceTo = TraceEnd;
		}
	}

}

void AWeapon::OnRep_HitScanTrace()
{
	PlayImpactFX(HitScanTrace.TraceTo);
	PlayTracerFX(HitScanTrace.TraceTo);
	PlayMuzzleFX();
}

void AWeapon::SetADS(bool ADS)
{
	if (Role < ROLE_Authority)
	{
		ServerSetADS(ADS);
	}
	bIsADS = ADS;
}

void AWeapon::ServerFire_Implementation()
{
	Fire();
}

bool AWeapon::ServerFire_Validate()
{
	return true;
}

void AWeapon::ServerSetADS_Implementation(bool ADS)
{
	SetADS(ADS);
}

bool AWeapon::ServerSetADS_Validate(bool ADS)
{
	return true;
}

void AWeapon::PlayWeaponAnimation(UAnimationAsset *Anim, bool bLoop)
{
	if (Anim)
	{
		GetMesh()->PlayAnimation(Anim, bLoop);
	}
}

void AWeapon::PlayMuzzleFX()
{
	if (WeaponConfig.MuzzleFX)
	{
		UParticleSystemComponent *SpawnedFX = UGameplayStatics::SpawnEmitterAttached(WeaponConfig.MuzzleFX,
			GetMesh(), FName("Muzzle"));
	}
}

void AWeapon::PlayImpactFX(FVector Loc)
{
	if (WeaponConfig.ImpactFX)
	{
		UParticleSystemComponent *SpawnedFX = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
			WeaponConfig.ImpactFX, Loc, FRotator::ZeroRotator, true);
	}
}

void AWeapon::PlayTracerFX(FVector Loc)
{
	if (WeaponConfig.TracerFX)
	{
		UParticleSystemComponent *SpawnedFX = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
			WeaponConfig.TracerFX, GetMesh()->GetSocketLocation("Muzzle"), FRotator::ZeroRotator, true);
		if (SpawnedFX)
		{
			SpawnedFX->SetVectorParameter("Target", Loc);
		}
	}
}

void AWeapon::PlayWeaponRecoil(TSubclassOf<UCameraShake> Recoil)
{
	if (Recoil)
	{
		AMBaseCharacter *Char = Cast<AMBaseCharacter>(GetOwner());
		APlayerController *PC = Cast<APlayerController>(Char->GetController());
		if (PC)
		{
			PC->ClientPlayCameraShake(Recoil);
		}
	}
}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AWeapon, HitScanTrace, COND_SkipOwner);
	DOREPLIFETIME(AWeapon, bIsADS);
}
