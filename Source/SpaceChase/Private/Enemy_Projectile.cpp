// Fill out your copyright notice in the Description page of Project Settings.


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


	if (ProjectileMovement)
	{
		ProjectileMovement->InitialSpeed=1000.0f;
		ProjectileMovement->MaxSpeed=1000.0f;
		ProjectileMovement->bRotationFollowsVelocity = true;
		ProjectileMovement->bShouldBounce = false;
		ProjectileMovement->ProjectileGravityScale = 0;
		ProjectileMovement->bRotationFollowsVelocity = false;	
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
	GetWorldTimerManager().SetTimer(Death, this, &AEnemy_Projectile::Die, 4.0f, false);
}

// Called every frame
void AEnemy_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy_Projectile::Die()
{
	
	Destroy();
        
	

}

void AEnemy_Projectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Your logic when an overlap begins
	if (OtherActor && OtherActor->ActorHasTag("Bullet"))
	{
		Die();
        
	}

	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		Die();
        
	}

}