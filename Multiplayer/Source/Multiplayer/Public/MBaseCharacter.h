// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MBaseCharacter.generated.h"

class AWeapon;

UCLASS()
class MULTIPLAYER_API AMBaseCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent *Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent *SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UHealthComponent *HealthComp;

public:
	// Sets default values for this character's properties
	AMBaseCharacter();

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	TSubclassOf<AWeapon> DefaultWeapon;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = Dead)
	bool bIsDead;

	UPROPERTY(EditDefaultsOnly, Category = Player)
	float ZoomedFOV;

	UPROPERTY(EditDefaultsOnly, Category = Player, meta = (ClampMin = 0.1, ClampMax = 100))
	float ZoomInterpSpeed;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = Player)
	float ControlPitchRotation;

	void ClientSetPitchRotation();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetPitchRotation();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetMaxWalkSpeed(float Speed);

protected:
#pragma region UE4 LIFECYCLE
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

#pragma endregion

	//////////////*BASE CHARACTER MOVEMENT*////////////////
	void MoveRight(float Value);

	void MoveForward(float Value);

	void LookUp(float Value);

	void TurnRight(float Value);

	void BeginCrouch();

	void EndCrouch();

	void StartAim();

	void EndAim();
	////////////////*END BASE CHARACTER MOVEMENT*///////////

	void StartFire();

	void ExitFire();

	bool bWantsToZoom;

	void ZoomHandling();

	float DefaultFOV;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	AWeapon *CurrentWeapon;


	UFUNCTION()
	void OnHandleDamage(UHealthComponent *OwningHealthComp, AActor* DamagedActor, float Damage,
		const class UDamageType* DamageType, class AController* InstigatedBy,
		AActor* DamageCauser);

public:	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetActorEyesViewPoint(FVector& out_Location, FRotator& out_Rotation) const override;

	FORCEINLINE UCameraComponent *GetCamera() const { return Camera; }
	FORCEINLINE USpringArmComponent *GetSpringArmComp() const { return SpringArmComp; }
	FORCEINLINE UHealthComponent *GetHealthComp() const { return HealthComp; }
	
};
