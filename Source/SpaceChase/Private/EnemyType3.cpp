// Fill out your copyright notice in the Description page of Project Settings.


#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "PlayablePlayer.h" 
#include "EnemyType3.h"

#include "PlayablePlayer.h"

// Sets default values
AEnemyType3::AEnemyType3()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

Tags.Add(FName("Enemy"));

    
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



	if (SkeletalMesh)
	{
    
		SkeletalMesh->CastShadow = false;
	}

	
	if (ProjectileMovement)
	{
		ProjectileMovement->InitialSpeed = 400.f;
		ProjectileMovement->MaxSpeed = 500.f;
		ProjectileMovement->ProjectileGravityScale = 0.f;
		ProjectileMovement->HomingAccelerationMagnitude = 4000.f;
	}

}

// Called when the game starts or when spawned
void AEnemyType3::BeginPlay()
{
	Super::BeginPlay();
	PlayerReff = Cast<APlayablePlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	GetWorldTimerManager().SetTimer(DelayTimerHandle, this, &AEnemyType3::DoPattern, 0.5f, false);
	GetWorldTimerManager().SetTimer(StartShouting, this, &AEnemyType3::ShoutPlayer, 2.0f, false);
}

// Called every frame
void AEnemyType3::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	
}


void AEnemyType3::DoPattern()
{
	
	GetPattern(Poss);

}





void AEnemyType3::ShoutPlayer()
{
    int RandNo = FMath::FRandRange(0.0f, 100.0f);
    if (!PlayerReff || !FireSpot) return;




    if (RandNo>30 && ProjectileMovement->bIsHomingProjectile ==false)   
    {

    const FVector From = FireSpot->GetComponentLocation();
    const FVector To = PlayerReff->GetMesh()->GetComponentLocation();
    const FVector Direction = (To - From).GetSafeNormal();
        FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        GetWorld()->SpawnActor<AActor>(EnemyBullet, From, Direction.Rotation(), SpawnParams);
    }
        GetWorldTimerManager().SetTimer(StartShouting, this, &AEnemyType3::ShoutPlayer, 1.0f, false);
}


void AEnemyType3::Die()
{
	Destroy();
}