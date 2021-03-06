// Simon Sejer Nielsen 2018

#include "Mannequin.h"
#include "Gun.h"


// Sets default values
AMannequin::AMannequin()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Camera Setup
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(RootComponent);
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// First Person Mesh Setup
	FPMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPMesh"));
	FPMesh->SetupAttachment(FirstPersonCameraComponent);
	FPMesh->SetOnlyOwnerSee(true);
	FPMesh->bCastDynamicShadow = false;
	FPMesh->CastShadow = false;
	FPMesh->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	FPMesh->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

}

// Called when the game starts or when spawned
void AMannequin::BeginPlay()
{
	Super::BeginPlay();

	if (WeaponBlueprint == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon Blueprint Missing!"))
		return;
	}

	Weapon = GetWorld()->SpawnActor<AGun>(WeaponBlueprint);
	if (IsPlayerControlled())
	{
		Weapon->AttachToComponent(FPMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint")); //Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor/OnPossesed
	}
	else
	{
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint")); // Setting the gun for the AI so it appears in the right slot and not the first person arms
	}
	Weapon->ThirdPersonAnimInstance = GetMesh()->GetAnimInstance(); // Gets the first found mesh
	Weapon->FirstPersonAnimInstance = FPMesh->GetAnimInstance();
}

// Called every frame
void AMannequin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMannequin::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMannequin::PullTrigger);

	// If we want to bind it to an event on a different class do below
	// InputComponent->BindAction("Fire", IE_Pressed, Weapon, &Gun::OnFire);	// Its a &Gun:OnFire because the class name is Gun - Name this "weapon" or "tool" in own game
}

void AMannequin::PullTrigger()
{
	if (!Weapon){return;}
	Weapon->OnFire();
}

void AMannequin::UnPossessed() 
{
	Super::UnPossessed(); // apparently this is run before begin play
	if (!Weapon) // Make a "Death" function to reattach the weapon to a third person slot would probably be preferable.
	{
		return;
	}
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint")); // Setting the gun for the AI so it appears in the right slot and not the first person arms
}
