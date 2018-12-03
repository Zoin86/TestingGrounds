// Simon Sejer Nielsen 2018

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Tile.generated.h"

UCLASS()
class TESTINGGROUNDS_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void PlaceActors(TSubclassOf<AActor> ToSpawn, int32 MinSpawn = 1, int32 MaxSpawn = 1, float MinRotation = 0, float MaxRotation = 0, float Radius = 500.f);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	UBoxComponent* PropSpawnBox;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	bool FindEmptyLocation(FVector &OutLocation, float Radius); // The FVector& OutLocation will allow it to fail (since its a bool function) but still return the FVector in case it does NOT fail.

	void PlaceActor(TSubclassOf<AActor> ToSpawn, FVector SpawnPoint, FRotator Rotation);

	bool CanSpawnAtLocation(FVector Location, float Radius);
};