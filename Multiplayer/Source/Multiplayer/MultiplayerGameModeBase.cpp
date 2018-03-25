// Fill out your copyright notice in the Description page of Project Settings.

#include "MultiplayerGameModeBase.h"
#include "GameFramework/PlayerController.h"
#include "Announcer/MAnnouncer.h"
#include "GameInstance/MGameInstance.h"
#include "Sound/SoundCue.h"



AMultiplayerGameModeBase::AMultiplayerGameModeBase()
{

}

void AMultiplayerGameModeBase::PlayAnnoucerSound(USoundCue *Sound)
{
	if (Sound)
	{
		for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			if (!(Iterator->IsValid())) continue;

			Iterator->Get()->ClientPlaySound(Sound);
		}
	}
}

void AMultiplayerGameModeBase::RespawnDeadPlayer()
{
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		if (!(Iterator->IsValid())) continue;

		APlayerController *PC = Iterator->Get();
		if (PC && PC->GetPawn() == nullptr)
		{
			RestartPlayer(PC);
		}
	}
}

