// Simon Sejer Nielsen 2018

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/ChildActorComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "Mannequin.generated.h"

UCLASS()
class TESTINGGROUNDS_API AMannequin : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMannequin();

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<class AGun> WeaponBlueprint;

	UFUNCTION(BlueprintCallable, Category = "Weapon") // consider putting this under protected
	void PullTrigger();

private:
	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	class USkeletalMeshComponent* FPMesh;

	/**Child Actor Component */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Component", Meta = (AllowPrivateAccess = "true"))
	class UChildActorComponent* WeaponComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	AGun* Weapon;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	
};
