// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(MULTIPLAYER), meta=(BlueprintSpawnableComponent) )
class MULTIPLAYER_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	int32 DefaultHealth;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Health")
	int32 Health;
		
	
};
