// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/Character.h"

#include "Kismet/GameplayStatics.h"

#include "Enemy_Projectile.h"

// Sets default values
AEnemy_Projectile::AEnemy_Projectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	

		ProjectileMovement= CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("P_Movement"));
	StaticMesh= CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	BoxCollider= CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RotatingMovement= CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Rotate"));


	/*
	SetRootComponent(StaticMesh);
	*/
	BoxCollider->SetupAttachment(StaticMesh);

	
	if (StaticMesh)
	{
    
		StaticMesh->CastShadow = false;
	}


	if (ProjectileMovement)
	{
		ProjectileMovement->InitialSpeed=1000.0f;
		ProjectileMovement->MaxSpeed=1000.0f;
		ProjectileMovement->ProjectileGravityScale = 0;
		ProjectileMovement->bRotationFollowsVelocity = false;	
		ProjectileMovement->HomingAccelerationMagnitude = 4000.f;
	}



	
	if (BoxCollider)
	{
		BoxCollider->SetHiddenInGame(true,true);
	}
	
}

// Called when the game starts or when spawned
void AEnemy_Projectile::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add(FName("Enemy"));
	PlayerReff = Cast<ACharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	home();
}

// Called every frame
void AEnemy_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy_Projectile::home()
{
	if (!PlayerReff ) {return;}

	float MyLoc = GetActorLocation().X;
	float PlayerLoc = PlayerReff->GetActorLocation().X;
	float Dist = MyLoc - PlayerLoc;

	ProjectileMovement->HomingTargetComponent = PlayerReff->GetMesh();
	ProjectileMovement->bIsHomingProjectile = true;


	if (Dist < 400.f)
	{
		GetWorldTimerManager().SetTimer(Death, this, &AEnemy_Projectile::Die, 4.0f, false);	
		ProjectileMovement->bIsHomingProjectile = false;
        
	}
	GetWorldTimerManager().SetTimer(homing, this, &AEnemy_Projectile::home, 0.1f, false);
}


void AEnemy_Projectile::Die()
{
	
	Destroy();
        
	

}

