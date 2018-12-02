// Simon Sejer Nielsen 2018

#include "Tile.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int32 MinSpawn, int32 MaxSpawn, float MinRotation, float MaxRotation)
{
	FVector Min(0.f, -2000.f, 0.f);
	FVector Max(4000.f, 2000.f, 0.f);
	FBox Bounds(Min, Max);

	int32 NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);
	for (size_t i = 0; i < NumberToSpawn; i++)
	{
		FVector SpawnPoint = FMath::RandPointInBox(Bounds);
		AActor* Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn); //Doesn't need a type parameter, but I put it in so we know what we are passing in
		FRotator Rotation(FMath::RandRange(MinRotation, MaxRotation), FMath::RandRange(MinRotation, MaxRotation), FMath::RandRange(MinRotation, MaxRotation));

		Spawned->SetActorLocation(SpawnPoint);
		Spawned->SetActorRotation(Rotation);
		Spawned->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
		
	}
	
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
	
	CastSphere(GetActorLocation() + FVector(0, 0, 1000), 300);
	CastSphere(GetActorLocation(), 300);
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ATile::CastSphere(FVector Location, float Radius)
{
	FHitResult HitResult;
	bool bHasHit = GetWorld()->SweepSingleByChannel(HitResult, Location, Location, FQuat::Identity, ECollisionChannel::ECC_Camera, FCollisionShape::MakeSphere(Radius));
	
	FColor ResultColor = bHasHit ? FColor::Red : FColor::Green;

	DrawDebugSphere(GetWorld(), Location, Radius, 32, ResultColor, true, 100.0f, 0.0f, 5.0f);
	
	return bHasHit;
}

