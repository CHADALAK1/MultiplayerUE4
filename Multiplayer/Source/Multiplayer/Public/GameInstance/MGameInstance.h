// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MGameInstance.generated.h"

class AMAnnouncer;

class USoundCue;

USTRUCT(BlueprintType)
struct FAnnoucerSounds
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, Category = Config)
	USoundCue *EnemyKilled;

	UPROPERTY(EditDefaultsOnly, Category = Config)
	USoundCue *EnemyDoubleKilled;

	UPROPERTY(EditDefaultsOnly, Category = Config)
	USoundCue *EnemyTripleKilled;

	UPROPERTY(EditDefaultsOnly, Category = Config)
	USoundCue *EnemyMonsterKilled;

	UPROPERTY(EditDefaultsOnly, Category = Config)
	USoundCue *EnemyUltraKilled;

	UPROPERTY(EditDefaultsOnly, Category = Config)
	USoundCue *EnemyUnstoppable;

	UPROPERTY(EditDefaultsOnly, Category = Config)
	USoundCue *EnemyHolyShit;

	UPROPERTY(EditDefaultsOnly, Category = Config)
	USoundCue *ClientKilled;

	UPROPERTY(EditDefaultsOnly, Category = Config)
	USoundCue *ClientStreakKilled;
};

/**
 * 
 */
UCLASS()
class MULTIPLAYER_API UMGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds)
	FAnnoucerSounds AnnouncerSounds;
	
	
};
