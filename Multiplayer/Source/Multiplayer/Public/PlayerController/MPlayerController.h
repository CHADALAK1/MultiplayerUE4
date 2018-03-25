// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYER_API AMPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	AMPlayerController();

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Kills)
	int32 KillStreak;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Kills)
	int32 Kills;

	UPROPERTY(Replicated,EditAnywhere, BlueprintReadWrite, Category = Kills)
	int32 Deaths;
	
	void AddKill();

	void ClearKillCount();

};
