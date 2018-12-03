// Simon Sejer Nielsen 2018

#include "Tile.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int32 MinSpawn, int32 MaxSpawn, float MinRotation, float MaxRotation, float Radius)
{
	int32 NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);
	for (size_t i = 0; i < NumberToSpawn; i++)
	{
		FRotator Rotation(FMath::RandRange(MinRotation, MaxRotation), FMath::RandRange(MinRotation, MaxRotation), FMath::RandRange(MinRotation, MaxRotation));
		FVector SpawnPoint;
		bool bFound = FindEmptyLocation(SpawnPoint, Radius);
		if (bFound)			// Because FindEmptyLocation(FVector& OutLocation, Radius) has the out parameter, it'll allow us to check the bool while also Setting SpawnPoint to the &Outlocation - Remember the Ampersand for outparameters
		{
			PlaceActor(ToSpawn, SpawnPoint, Rotation); 
		}
	}
}

bool ATile::FindEmptyLocation(FVector &OutLocation, float Radius)
{
	FVector Min(0.f, -2000.f, 0.f);
	FVector Max(4000.f, 2000.f, 0.f);
	FBox Bounds(Min, Max);
	const int32 MAX_ATTEMPTS = 36;
	for (int i = 0; i < MAX_ATTEMPTS; i++)
	{
		FVector CandidatePoint = FMath::RandPointInBox(Bounds);
		if (CanSpawnAtLocation(CandidatePoint, Radius))
		{
			OutLocation = CandidatePoint; // This will set the OutLocation Parameter, which we use in PlaceActors
			return true;
		}
	}
	return false;
}

void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, FVector SpawnPoint, FRotator Rotation)
{
	AActor* Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn); //Doesn't need a type parameter, but I put it in so we know what we are passing in
	Spawned->SetActorLocation(SpawnPoint);
	Spawned->SetActorRotation(Rotation);
	Spawned->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ATile::CanSpawnAtLocation(FVector Location, float Radius)
{
	FHitResult HitResult;
	FVector GlobalLocation = ActorToWorld().TransformPosition(Location);

	// TODO - Will have to look up the stuff on FQuat::Identity again to understand it better - Lecture 313 - 314 (One of these two)
	bool bHasHit = GetWorld()->SweepSingleByChannel(HitResult, GlobalLocation, GlobalLocation, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel2, FCollisionShape::MakeSphere(Radius));
	
	FColor ResultColor = bHasHit ? FColor::Red : FColor::Green;

	DrawDebugCapsule(GetWorld(), GlobalLocation, 100, Radius, FQuat::Identity, ResultColor, true, 100.0f);
	
	return !bHasHit;
}

