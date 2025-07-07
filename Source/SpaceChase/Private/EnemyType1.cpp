// Fill out your copyright notice in the Description page of Project Settings.


#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "EnemyType1.h"


// Sets default values
AEnemyType1::AEnemyType1()
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
void AEnemyType1::BeginPlay()
{
    Super::BeginPlay();


    
    PlayerReff = Cast<ACharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));


    
    FVector RandomDirection = FMath::VRand();
    float Distance = FMath::FRandRange(250.0f, 550.f); 
    FVector Offset = RandomDirection * Distance;
    FVector NewLocation = GetActorLocation() + Offset;
    SetActorLocation(NewLocation);



    GetWorldTimerManager().SetTimer(DelayTimerHandle, this, &AEnemyType1::GoToLocation, 1.0f, false);


}

// Called every frame
void AEnemyType1::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}
void AEnemyType1::GoToLocation()
{
    if (!PlayerReff || Run) {return;}

    // Get distance
     float MyLoc = GetActorLocation().X;
     float PlayerLoc = PlayerReff->GetActorLocation().X;
    float Dist = MyLoc - PlayerLoc;

    if (Dist < 6000.f)
    {
        ProjectileMovement->HomingTargetComponent = PlayerReff->GetMesh();
        ProjectileMovement->bIsHomingProjectile = true;
        if (!Shoot)
        {
    ShoutPlayer();
            Shoot=true;
        }

        // Schedule to stop homing after 3 sec (instead of using Delay in Blueprint)
    }
    if (Dist < 400.f)
    {
        ProjectileMovement->bIsHomingProjectile = false;
        Run = true;
        
        GetWorldTimerManager().SetTimer(death, this, &AEnemyType1::Die, 2.0f, false);
    }
        GetWorldTimerManager().SetTimer(DelayTimerHandle, this, &AEnemyType1::GoToLocation, 0.01f, false);

}
void AEnemyType1::Die()
{
    if (Run==true )
    {
    Destroy();
        
    }

}


void AEnemyType1::ShoutPlayer()
{
    int RandNo = FMath::FRandRange(0.0f, 100.0f);
    if (!PlayerReff || !FireSpot) return;




    if (RandNo>60 && Run ==false)   
    {

    const FVector From = FireSpot->GetComponentLocation();
    const FVector To = PlayerReff->GetMesh()->GetComponentLocation();
    const FVector Direction = (To - From).GetSafeNormal();
        FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        GetWorld()->SpawnActor<AActor>(EnemyBullet, From, Direction.Rotation(), SpawnParams);
    }
        GetWorldTimerManager().SetTimer(StartShouting, this, &AEnemyType1::ShoutPlayer, 1.0f, false);
}


