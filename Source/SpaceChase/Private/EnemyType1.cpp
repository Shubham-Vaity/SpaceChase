// Fill out your copyright notice in the Description page of Project Settings.


#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "EnemyType1.h"


// Sets default values
AEnemyType1::AEnemyType1()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Default"));
	SkeletalMesh=CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	ProjectileMovement=CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("P_Movement"));
	hitBox=CreateDefaultSubobject<USphereComponent>(TEXT("HitBox"));
	FireSpot=CreateDefaultSubobject<UArrowComponent>(TEXT("FireSpot"));

	
	//Paraent Chield
	 SetRootComponent(DefaultSceneRoot);
	 SkeletalMesh->SetupAttachment(DefaultSceneRoot);
    hitBox->SetupAttachment(DefaultSceneRoot);
	FireSpot->SetupAttachment(DefaultSceneRoot);


	if (ProjectileMovement)
	{
		ProjectileMovement->InitialSpeed = 0.f;
		ProjectileMovement->MaxSpeed = 400.f;
		ProjectileMovement->ProjectileGravityScale = 0.f;
		ProjectileMovement->HomingAccelerationMagnitude = 500.f;
	}

	
}

// Called when the game starts or when spawned
void AEnemyType1::BeginPlay()
{
	Super::BeginPlay();
	PlayerReff = Cast<ACharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
	GetWorldTimerManager().SetTimer(DelayTimerHandle, this, &AEnemyType1::GoToLocation, 3.0f, false);

	
}

// Called every frame
void AEnemyType1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyType1::GoToLocation()
{
	if (!PlayerReff || Run) return;

	// Get distance
	const FVector MyLoc = GetActorLocation();
	const FVector PlayerLoc = PlayerReff->GetActorLocation();
	float Dist = FVector::Dist(MyLoc, PlayerLoc);

	if (Dist > 1000.f) // use your desired distance check
	{
		ProjectileMovement->HomingTargetComponent = PlayerReff->GetRootComponent();
		ProjectileMovement->bIsHomingProjectile = true;

		// Schedule to stop homing after 3 sec (instead of using Delay in Blueprint)
		GetWorldTimerManager().SetTimer(DelayTimerHandle, this, &AEnemyType1::GoToLocation, 3.0f, false);
	}
}

void AEnemyType1::StopHomingNow()
{
	if (!ProjectileMovement) return;

	ProjectileMovement->bIsHomingProjectile = false;
	Run = true;

	// Lock to a socket (optional)
	if (PlayerReff)
	{
		AttachToComponent(PlayerReff->GetMesh(), FAttachmentTransformRules::KeepWorldTransform, FName("SocketName")); // Replace "SocketName"
	}
}

void AEnemyType1::ShoutPlayer()
{
	if (!PlayerReff || !FireSpot) return;

	const FVector From = FireSpot->GetComponentLocation();
	const FVector To = PlayerReff->GetActorLocation();

	DrawDebugLine(GetWorld(), From, To, FColor::Red, false, 2.f, 0, 2.f);
}

