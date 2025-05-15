// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/BoxComponent.h"
#include "Floor.h"

// Sets default values
AFloor::AFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Default"));

	MyStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor"));
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BOX"));
	
	SetRootComponent(DefaultSceneRoot);
	RootComponent = DefaultSceneRoot;

}

// Called when the game starts or when spawned
void AFloor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

