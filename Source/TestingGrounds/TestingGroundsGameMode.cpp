// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "TestingGroundsGameMode.h"
#include "TestingGroundsHUD.h"
#include "Player/FirstPersonCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATestingGroundsGameMode::ATestingGroundsGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Dynamic/Player/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ATestingGroundsHUD::StaticClass();

	// Create Actor Pool
	ActorPool = CreateDefaultSubobject<UActorPool>(FName("Actor Pool"));
}

void ATestingGroundsGameMode::PopulateBoundsVolumePool()
{

	TArray<AActor*> FoundActors;
													// The class we are trying to find
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANavMeshBoundsVolume::StaticClass(), FoundActors); // Gets all navmeshs in world

	UE_LOG(LogTemp, Warning, TEXT("Number of Found Actors: %i"), FoundActors.Num())

	for (auto Actor : FoundActors)
	{
		auto FoundActor = Cast<ANavMeshBoundsVolume>(Actor);
		AddToPool(FoundActor);
	}

	/// Using GetAllActorsOfClass() like above is the better solution for this.
	/*TActorIterator<AActor> ActorIterator = TActorIterator<AActor>(GetWorld());
	while (ActorIterator)
	{
		AActor* FoundActor = *ActorIterator; // TODO - Need to look up this again for explanation as to we are using a pointer on the ActorIterator
		UE_LOG(LogTemp, Warning, TEXT("Found Actor: %s"), *FoundActor->GetName())
		++ActorIterator; // Need to do ++ infront to increment.
	}*/
}

void ATestingGroundsGameMode::AddToPool(ANavMeshBoundsVolume* VolumeToAdd)
{
	ActorPool->Add(VolumeToAdd);
}
