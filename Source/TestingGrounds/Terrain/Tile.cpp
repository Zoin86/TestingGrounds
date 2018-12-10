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

}

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int32 MinSpawn, int32 MaxSpawn, float MinYaw, float MaxYaw, float MinPitch, float MaxPitch, float MinRoll, float MaxRoll, float Radius, float MinScale, float MaxScale)
{
	int32 NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);
	for (size_t i = 0; i < NumberToSpawn; i++)
	{
		FVector SpawnPoint;

		float RandomScale = FMath::RandRange(MinScale, MaxScale);
		
		float RandomPitch = FMath::RandRange(MinPitch, MaxPitch);
		float RandomYaw = FMath::RandRange(MinYaw, MaxYaw);
		float RandomRoll = FMath::RandRange(MinRoll, MaxRoll);

		bool bFound = FindEmptyLocation(SpawnPoint, Radius * RandomScale); // Multiply Radius with Randomscale so the Sphere Radius check scaled to the proportion of the mesh random scale.
		if (bFound)			// Because FindEmptyLocation(FVector& OutLocation, Radius) has the out parameter, it'll allow us to check the bool while also Setting SpawnPoint to the &Outlocation - Remember the Ampersand for outparameters
		{
			FRotator RandomRotation(RandomPitch, RandomYaw, RandomRoll);
			PlaceActor(ToSpawn, SpawnPoint, RandomRotation, RandomScale); 
		}
	}
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
	UE_LOG(LogTemp, Warning, TEXT("[%s] Checked out: {%s}"), *GetName(), *NavMeshBoundsVolume->GetName())
	NavMeshBoundsVolume->SetActorLocation(GetActorLocation());
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

void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, FVector SpawnPoint, FRotator Rotation, float Scale)
{
	AActor* Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn); //Doesn't need a type parameter, but I put it in so we know what we are passing in
	Spawned->SetActorLocation(SpawnPoint);
	Spawned->SetActorRotation(Rotation);
	Spawned->SetActorScale3D(FVector(Scale));
	Spawned->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();

}

void ATile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	ActorPool->Return(NavMeshBoundsVolume);

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

	//DrawDebugCapsule(GetWorld(), GlobalLocation, 100, Radius, FQuat::Identity, ResultColor, true, 100.0f);
	
	return !bHasHit;
}

