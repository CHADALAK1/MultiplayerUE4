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
	////////////////*END BASE CHARACTER MOVEMENT*///////////

	void StartFire();

	void ExitFire();

	AWeapon *CurrentWeapon;


	UFUNCTION()
	void OnHandleDamage(UHealthComponent *OwningHealthComp, AActor* DamagedActor, float Damage,
		const class UDamageType* DamageType, class AController* InstigatedBy,
		AActor* DamageCauser);

public:	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE UCameraComponent *GetCamera() const { return Camera; }
	FORCEINLINE USpringArmComponent *GetSpringArmComp() const { return SpringArmComp; }
	FORCEINLINE UHealthComponent *GetHealthComp() const { return HealthComp; }
	
};
