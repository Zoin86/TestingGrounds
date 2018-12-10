// Simon Sejer Nielsen 2018

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActorPool.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTINGGROUNDS_API UActorPool : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActorPool();

	AActor* Checkout();
	void Return(AActor* ActorToReturn);
	void Add(AActor* ActorToAdd);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	TArray<AActor*> Pool;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
