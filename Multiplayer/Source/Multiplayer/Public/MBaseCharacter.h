// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MBaseCharacter.generated.h"

UCLASS()
class MULTIPLAYER_API AMBaseCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent *Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent *SpringArmComp;

public:
	// Sets default values for this character's properties
	AMBaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//////////////*BASE CHARACTER MOVEMENT*////////////////
	void MoveRight(float Value);

	void MoveForward(float Value);

	void LookUp(float Value);

	void TurnRight(float Value);

	void BeginCrouch();

	void EndCrouch();
	////////////////*END BASE CHARACTER MOVEMENT*///////////

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE UCameraComponent *GetCamera() const { return Camera; }
	FORCEINLINE USpringArmComponent *GetSpringArmComp() const { return SpringArmComp; }
	
};
