// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class AMBaseCharacter;

UCLASS()
class MULTIPLAYER_API AWeapon : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent *Mesh;
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	void StartFire();

	void EndFire();

	void SetCurrentOwner(AMBaseCharacter *NewOwner);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Fire();

	AMBaseCharacter *MyOwner;

public:

	FORCEINLINE USkeletalMeshComponent *GetMesh() const { return Mesh; }
	
};
