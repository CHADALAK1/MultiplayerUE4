// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class AMBaseCharacter;
class UAnimationAsset;
class UCameraShake;

USTRUCT()
struct FHitScanTrace
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY()
	FVector_NetQuantize TraceFrom;

	UPROPERTY()
	FVector_NetQuantize TraceTo;
};

USTRUCT(BlueprintType)
struct FWeaponConfig
{
	GENERATED_USTRUCT_BODY()

	/*RPM for the weapon (Ex. 600 bullets per minute)*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	float RateOfFire;

	/*Weapon spread when weapon is firing from the hip*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	float HipWeaponSpread;

	/*Weapon spread when Weapon is firing when Aiming*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	float AimWeaponSpread;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	UAnimationAsset *FireAnimation;

	UPROPERTY(EditDefaultsOnly, Category = Config)
	TSubclassOf<UCameraShake> WeaponRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	UParticleSystem *MuzzleFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	UParticleSystem *TracerFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	UParticleSystem *ImpactFX;

};

UCLASS()
class MULTIPLAYER_API AWeapon : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent *Mesh;
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	FWeaponConfig WeaponConfig;

	void StartFire();

	void EndFire();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	float TimeBetweenShots;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bIsADS;

	UPROPERTY(ReplicatedUsing = OnRep_HitScanTrace)
	FHitScanTrace HitScanTrace;

	UFUNCTION()
	void OnRep_HitScanTrace();

	void SetADS(bool ADS);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Fire();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerFire();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetADS(bool ADS);

	void PlayWeaponAnimation(UAnimationAsset *Anim, bool bLoop);

	void PlayMuzzleFX();

	void PlayImpactFX(FVector Loc);

	void PlayTracerFX(FVector Loc);

	void PlayWeaponRecoil(TSubclassOf<UCameraShake> Recoil);

	UPROPERTY(EditDefaultsOnly, Category = WeaponConfig)
	TSubclassOf<UDamageType> DamageType;

	FTimerHandle TimeBetweenShotsHandle;

public:

	FORCEINLINE USkeletalMeshComponent *GetMesh() const { return Mesh; }
	
};
