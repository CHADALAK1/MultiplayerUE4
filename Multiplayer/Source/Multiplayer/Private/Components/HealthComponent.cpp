// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	DefaultHealth = 100;
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = DefaultHealth;

	AActor *HealthOwner = GetOwner();

	if (HealthOwner)
	{
		HealthOwner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnHandleDamage);
	}

}

void UHealthComponent::OnHandleDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, 
	class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f)
		return;

	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);

	UE_LOG(LogTemp, Log, TEXT("%s"), *FString::SanitizeFloat(Health));

	TakeDamage.Broadcast(this, DamagedActor, Damage, DamageType, InstigatedBy, DamageCauser);
}