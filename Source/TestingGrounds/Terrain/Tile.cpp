// Simon Sejer Nielsen 2018

#include "Tile.h"
#include "AI/NavigationSystemBase.h"
#include "EngineUtils.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MinSpawningExtent = FVector(0.f, -2000.f, 0.f);
	MaxSpawningExtent = FVector(4000.f, 2000.f, 0.f);
	NavigationBoundsLocationOffset = FVector(2000, 0, 0);
}

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int32 MinSpawn, int32 MaxSpawn, float MinYaw, float MaxYaw, float MinPitch, float MaxPitch, float MinRoll, float MaxRoll, float Radius, float MinScale, float MaxScale)
{
	RandomlyPlaceActors(ToSpawn, MinSpawn, MaxSpawn, MinYaw, MaxYaw, MinPitch, MaxPitch, MinRoll, MaxRoll, Radius, MinScale, MaxScale);
}

void ATile::PlaceAIPawns(TSubclassOf<APawn> ToSpawn, int32 MinSpawn, int32 MaxSpawn, float Radius)
{
	RandomlyPlaceActors(ToSpawn, MinSpawn, MaxSpawn, 0, 0, 0, 0, 0, 0, Radius);
}

template<class T> // T is a generic type - because of this, the function will automatically figure out which type to use and which "PlaceActor" function use from that type - Make sure that this function is the top function or compile will fail due to the compiler thinking we are redefining
void ATile::RandomlyPlaceActors(TSubclassOf<T> ToSpawn, int32 MinSpawn, int32 MaxSpawn, float MinYaw, float MaxYaw, float MinPitch, float MaxPitch, float MinRoll, float MaxRoll, float Radius, float MinScale, float MaxScale)
{
	int32 NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);
	for (size_t i = 0; i < NumberToSpawn; i++)
	{
		FSpawnPosition SpawnPosition;
		SpawnPosition.Scale = FMath::RandRange(MinScale, MaxScale);

		float RandomPitch = FMath::RandRange(MinPitch, MaxPitch);
		float RandomYaw = FMath::RandRange(MinYaw, MaxYaw);
		float RandomRoll = FMath::RandRange(MinRoll, MaxRoll);

		/// Currently everything can overlap as no mesh has been spawned yet when trying to find an empty location - TODO Check to see if the location is valid.
		bool bFound = FindEmptyLocation(SpawnPosition.Location, Radius * SpawnPosition.Scale); // Multiply Radius with Scale so that the Sphere Radius check is scaled to the proportion of the mesh scale.
		if (bFound)			// Because FindEmptyLocation(FVector& OutLocation, Radius) has the out parameter, it'll allow us to check the bool while also Setting SpawnPoint to the &Outlocation - Remember the Ampersand for outparameters
		{
			SpawnPosition.Rotation = FRotator(RandomPitch, RandomYaw, RandomRoll);
			PlaceActor(ToSpawn, SpawnPosition);
		}
	}
}

void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, FSpawnPosition SpawnPosition)
{
	AActor* Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn);
	Spawned->SetActorRelativeLocation(SpawnPosition.Location);
	Spawned->SetActorRotation(SpawnPosition.Rotation);
	Spawned->SetActorScale3D(FVector(SpawnPosition.Scale));
	Spawned->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
}

void ATile::PlaceActor(TSubclassOf<APawn> ToSpawn, FSpawnPosition SpawnPosition)
{
	APawn* Spawned = GetWorld()->SpawnActor<APawn>(ToSpawn);
	Spawned->SetActorRelativeLocation(SpawnPosition.Location);
	Spawned->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	Spawned->SpawnDefaultController();
	Spawned->Tags.Add(FName("Guard"));
}

void ATile::SetPoolReference(UActorPool * ActorPoolToSet)
{
	ActorPool = ActorPoolToSet;

	PositionNavMeshBoundsVolume();
}

void ATile::PositionNavMeshBoundsVolume()
{
	NavMeshBoundsVolume = ActorPool->Checkout();
	if (NavMeshBoundsVolume == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] Not enough actors in pool!"), *GetName())
		return;
	}
	NavMeshBoundsVolume->SetActorLocation(GetActorLocation() + NavigationBoundsLocationOffset);
	FNavigationSystem::Build(*GetWorld());	
}

bool ATile::FindEmptyLocation(FVector &OutLocation, float Radius)
{
	FBox Bounds(MinSpawningExtent, MaxSpawningExtent);
	const int32 MAX_ATTEMPTS = 16;
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


// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();

}

void ATile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (ActorPool != nullptr && NavMeshBoundsVolume != nullptr)
	{
		ActorPool->Return(NavMeshBoundsVolume);
	}
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

	if (bVisualiseCanSpawn)
	{
		DrawDebugCapsule(GetWorld(), GlobalLocation, 100, Radius, FQuat::Identity, ResultColor, true, 100.0f);
	}
	return !bHasHit;
}

