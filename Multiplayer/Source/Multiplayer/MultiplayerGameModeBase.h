// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MultiplayerGameModeBase.generated.h"

class USoundCue;
class AMAnnouncer;

/**
 * 
 */
UCLASS()
class MULTIPLAYER_API AMultiplayerGameModeBase : public AGameMode
{
	GENERATED_BODY()
	
public:

	AMultiplayerGameModeBase();

	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundCue *BeginSound;

	UFUNCTION(BlueprintCallable, Category = Match)
	void PlayAnnoucerSound(USoundCue *Sound);
	
	void RespawnDeadPlayer();
};
