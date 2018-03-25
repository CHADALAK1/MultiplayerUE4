// Fill out your copyright notice in the Description page of Project Settings.

#include "MPlayerController.h"
#include "Sound/SoundCue.h"
#include "Net/UnrealNetwork.h"
#include "GameInstance/MGameInstance.h"




AMPlayerController::AMPlayerController()
{

}

void AMPlayerController::AddKill()
{
	KillStreak++;
	Kills++;
	UMGameInstance *GI = Cast<UMGameInstance>(GetGameInstance());
	if (GI)
	{
		switch (KillStreak)
		{
		case 1:
			ClientPlaySound(GI->AnnouncerSounds.EnemyKilled);
			break;
		case 2:
			ClientPlaySound(GI->AnnouncerSounds.EnemyDoubleKilled);
			break;
		case 3:
			ClientPlaySound(GI->AnnouncerSounds.EnemyTripleKilled);
			break;
		case 4:
			ClientPlaySound(GI->AnnouncerSounds.EnemyMonsterKilled);
			break;
		case 5:
			ClientPlaySound(GI->AnnouncerSounds.EnemyUltraKilled);
			break;
		case 10:
			ClientPlaySound(GI->AnnouncerSounds.EnemyHolyShit);
			break;
		default:
			ClientPlaySound(GI->AnnouncerSounds.EnemyUnstoppable);
			break;
		}
	}
}

void AMPlayerController::ClearKillCount()
{
	UMGameInstance *GI = Cast<UMGameInstance>(GetGameInstance());
	if (GI)
	{
		if (KillStreak >= 3)
		{
			ClientPlaySound(GI->AnnouncerSounds.ClientStreakKilled);
		}
		else
		{
			ClientPlaySound(GI->AnnouncerSounds.ClientKilled);
		}
	}
	KillStreak = 0;
	Deaths++;
}


void AMPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMPlayerController, Kills);
	DOREPLIFETIME(AMPlayerController, Deaths);
	DOREPLIFETIME(AMPlayerController, KillStreak);
}