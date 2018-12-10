// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "EngineUtils.h" // probably dont need this - since we're using a forloop
#include "Kismet/GameplayStatics.h"
#include "ActorPool.h"
#include "TestingGroundsGameMode.generated.h"

UCLASS(minimalapi)
class ATestingGroundsGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATestingGroundsGameMode();

	UFUNCTION(BlueprintCallable, Category = "Bounds Pool")
	void PopulateBoundsVolumePool();

private:
	void AddToPool(ANavMeshBoundsVolume* VolumeToAdd);

protected:
	UPROPERTY(BlueprintReadOnly)
	UActorPool* ActorPool;
};



