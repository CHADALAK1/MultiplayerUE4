// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthComponent.h"


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
}

