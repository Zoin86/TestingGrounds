// Simon Sejer Nielsen 2018

#include "ActorPool.h"

// Sets default values for this component's properties
UActorPool::UActorPool()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


AActor * UActorPool::Checkout()
{
	if (Pool.Num() == 0){return nullptr;}

	return Pool.Pop();
}

void UActorPool::Return(AActor* ActorToReturn)
{
	if (!ActorToReturn)
	{
		UE_LOG(LogTemp, Error, TEXT("Returned null actor") )
		return; 
	}
	Add(ActorToReturn);
	UE_LOG(LogTemp, Warning, TEXT("[%s] Actor Returned: {%s}"), *this->GetName(), *ActorToReturn->GetName())
}

void UActorPool::Add(AActor* ActorToAdd)
{
	if (!ActorToAdd)
	{
		UE_LOG(LogTemp, Error, TEXT("Added null actor!"))
		return;
	}
	Pool.Push(ActorToAdd);
	UE_LOG(LogTemp, Warning, TEXT("[%s] Actor Pushed: {%s}"), *this->GetName(), *ActorToAdd->GetName())
}

// Called when the game starts
void UActorPool::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UActorPool::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

