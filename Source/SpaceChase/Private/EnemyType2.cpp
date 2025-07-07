// Fill out your copyright notice in the Description page of Project Settings.


#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "EnemyType2.h"

// Sets default values
AEnemyType2::AEnemyType2()
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
		ProjectileMovement->InitialSpeed = 0.f;
		ProjectileMovement->MaxSpeed = 800.f;
		ProjectileMovement->ProjectileGravityScale = 0.f;
		ProjectileMovement->HomingAccelerationMagnitude = 1000.f;
	}

}

// Called when the game starts or when spawned
void AEnemyType2::BeginPlay()
{
	Super::BeginPlay();
	PlayerReff = Cast<ACharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));


	FVector RandomDirection = FMath::VRand();
	float Distance = FMath::FRandRange(250.0f, 550.f); 
	FVector Offset = RandomDirection * Distance;
	FVector NewLocation = GetActorLocation() + Offset;
	SetActorLocation(NewLocation);



	
	GetWorldTimerManager().SetTimer(DelayTimerHandle, this, &AEnemyType2::GoToLocation, 1.0f, false);

}

// Called every frame
void AEnemyType2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (PlayerReff && !Run)
	{
		FVector DirectionToPlayer = PlayerReff->GetActorLocation() - GetActorLocation();
		FRotator NewRotation = FRotationMatrix::MakeFromX(DirectionToPlayer).Rotator();
		
		SkeletalMesh->SetWorldRotation(NewRotation);

		
		FRotator MeshOffset = FRotator(0.f, -90.f, 0.f); 
		SkeletalMesh->AddRelativeRotation(MeshOffset);
	}
}

void AEnemyType2::GoToLocation()
{
    if (!PlayerReff || Run) {return;}

    // Get distance
     float MyLoc = GetActorLocation().X;
     float PlayerLoc = PlayerReff->GetActorLocation().X;
    float Dist = MyLoc - PlayerLoc;

    if (Dist < 8000.f)
    {
        ProjectileMovement->HomingTargetComponent = PlayerReff->GetMesh();
        ProjectileMovement->bIsHomingProjectile = true;
      
        // Schedule to stop homing after 3 sec (instead of using Delay in Blueprint)
    }
    if (Dist < 500.f)
    {
    	ProjectileMovement->bIsHomingProjectile = false;
    	Run = true;

    	if (!Shoot)
    	{
    		ShoutPlayer();
    		Shoot=true;
    	}


    	// Move straight in X for a few seconds
    	FVector Direction = FVector(1.f, 0.f, 0.f); 
    	ProjectileMovement->Velocity = Direction * 800.f;

    	// After delay, move down
    	GetWorldTimerManager().SetTimer(death, this, &AEnemyType2::Die, 6.0f, false);
    }
        GetWorldTimerManager().SetTimer(DelayTimerHandle, this, &AEnemyType2::GoToLocation, 0.01f, false);

}
void AEnemyType2::Die()
{
	if (Run)
	{
		Run = false;
		ProjectileMovement->Velocity = FVector(0.f, 0.f, -800.f);

		// Call DestroyEnemy after 2 seconds
		GetWorldTimerManager().SetTimer(death, this, &AEnemyType2::DestroyEnemy, 2.0f, false);
	}
}

void AEnemyType2::DestroyEnemy()
{
	Destroy();
}

void AEnemyType2::ShoutPlayer()
{
    int RandNo = FMath::FRandRange(0.0f, 100.0f);
    if (!PlayerReff || !FireSpot) return;




    if (RandNo>60 && ProjectileMovement->bIsHomingProjectile ==false)   
    {

    const FVector From = FireSpot->GetComponentLocation();
    const FVector To = PlayerReff->GetMesh()->GetComponentLocation();
    const FVector Direction = (To - From).GetSafeNormal();
        FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        GetWorld()->SpawnActor<AActor>(EnemyBullet, From, Direction.Rotation(), SpawnParams);
    }
        GetWorldTimerManager().SetTimer(StartShouting, this, &AEnemyType2::ShoutPlayer, 1.0f, false);
}


