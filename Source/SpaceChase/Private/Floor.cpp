// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/BoxComponent.h"
#include "PlayablePlayer.h" 
#include "Floor.h"

// Sets default values
AFloor::AFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Default"));
	MyStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor"));
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BOX"));
	FrountArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Frount"));
	

	SetRootComponent(DefaultSceneRoot);	
	MyStaticMesh->SetupAttachment(DefaultSceneRoot);
	BoxComponent->SetupAttachment(DefaultSceneRoot);
	FrountArrow->SetupAttachment(DefaultSceneRoot);



	//box things accoeding to chatgpt 
	BoxComponent->SetGenerateOverlapEvents(true);
	BoxComponent->SetCollisionProfileName("Trigger");

}

// Called when the game starts or when spawned
void AFloor::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AFloor::OnBoxOverlap);
	
}

// Called every frame
void AFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//box collider 

void AFloor::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		if (OtherActor->IsA(APlayablePlayer::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Player overlapped the floor!"));
			

				FVector SpawnLocation = FrountArrow->GetComponentLocation();
				FRotator SpawnRotation = FRotator::ZeroRotator;

				GetWorld()->SpawnActor<AFloor>(FloorToSpawn, SpawnLocation, SpawnRotation);
				UE_LOG(LogTemp, Warning, TEXT("Floor spawned"));
			
		}
	}
}


