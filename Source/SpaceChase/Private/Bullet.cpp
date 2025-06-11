// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Default"));
	MyStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor"));
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

	SetRootComponent(DefaultSceneRoot);
	MyStaticMesh->SetupAttachment(DefaultSceneRoot);

	if (MyStaticMesh) {

	}




	if(ProjectileMovement)
	{
		// ... other constructor code ...
	
		ProjectileMovement->InitialSpeed = 90000.f;
		ProjectileMovement->MaxSpeed = 90000.f;
		ProjectileMovement->bRotationFollowsVelocity = true;
		ProjectileMovement->bShouldBounce = false;
		ProjectileMovement->ProjectileGravityScale = 0;
	}

}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

