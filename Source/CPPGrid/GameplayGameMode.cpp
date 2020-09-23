// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "CheckpointPawn.h"
#include "HeroCharacter.h"
#include "HeroController.h"
#include "GameplayHUD.h"
#include "EnemyActor.h"

void AGameplayGameMode::BeginPlay()
{
	// GetPlayerCharacter and try to cast it to AHeroCharacter
	UWorld* World = GetWorld();
	if (World)
	{
		AHeroCharacter*  PlayerPointer = Cast<AHeroCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (PlayerPointer)
		{
			// If it succeds set player character reference
			PlayerCharacter = PlayerPointer;
			// And bind PlayerCharacterDeath event to listem to player death
			PlayerCharacter->PlayerDeath.AddDynamic(this, &AGameplayGameMode::PlayerCharacterDeath);
		}
	}
}

void AGameplayGameMode::PossesCheckpoint()
{
	APawn* CheckpointPawn = Cast<APawn>(UGameplayStatics::GetActorOfClass(GetWorld(), ACheckpointPawn::StaticClass()));
	if (CheckpointPawn)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			AHeroController* HeroController = Cast<AHeroController>(UGameplayStatics::GetPlayerController(World, 0));
			if (HeroController)
			{
				HeroController->Possess(CheckpointPawn);
				HeroController->ShowDeathMenu();
			}
		}
	}

}

void AGameplayGameMode::StopEnemyFire()
{
	TArray<AActor*> ActorArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyActor::StaticClass(), ActorArray);
	for (AActor* TActor : ActorArray)
	{
		AEnemyActor* EnemyActor = Cast<AEnemyActor>(TActor);
		if (EnemyActor)
		{
			EnemyActor->ResetEnemy();
		}
	}
}

void AGameplayGameMode::PlayerCharacterDeath()
{
	PlayerCharacter->SetActorHiddenInGame(true);
	StopEnemyFire();
	PossesCheckpoint();
	
}