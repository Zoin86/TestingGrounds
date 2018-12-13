// Simon Sejer Nielsen 2018

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "ActorPool.h"
#include "AI/Navigation/NavigationTypes.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "Tile.generated.h"

UCLASS()
class TESTINGGROUNDS_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void PlaceActors(TSubclassOf<AActor> ToSpawn, int32 MinSpawn = 1, int32 MaxSpawn = 1, float MinYaw = -180, float MaxYaw = 180, float MinPitch = 0, float MaxPitch = 0, float MinRoll = 0, float MaxRoll = 0, float Radius = 500.f, float MinScale = 1, float MaxScale = 1);
	
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void SetPoolReference(class UActorPool* ActorPoolToSet);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	FVector MinSpawningExtent;
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	FVector MaxSpawningExtent;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	FVector NavigationBoundsLocationOffset;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	UBoxComponent* PropSpawnBox;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	class UActorPool* ActorPool = nullptr;

	void PositionNavMeshBoundsVolume();

	bool FindEmptyLocation(FVector &OutLocation, float Radius); // The FVector& OutLocation will allow it to fail (since its a bool function) but still return the FVector in case it does NOT fail.

	void PlaceActor(TSubclassOf<AActor> ToSpawn, FVector SpawnPoint, FRotator Rotation, float Scale);

	bool CanSpawnAtLocation(FVector Location, float Radius);

	AActor* NavMeshBoundsVolume = nullptr;
};