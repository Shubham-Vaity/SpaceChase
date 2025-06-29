// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
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
	BoxComponent->SetupAttachment(MyStaticMesh);
	FrountArrow->SetupAttachment(MyStaticMesh);



	//box things accoeding to chatgpt 
	BoxComponent->SetGenerateOverlapEvents(true);
	BoxComponent->SetCollisionProfileName("Trigger");



	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/StarterContent/Shapes/Shape_Plane.Shape_Plane"));
	if (MeshAsset.Succeeded())
	{
		MyStaticMesh->SetStaticMesh(MeshAsset.Object);
		MyStaticMesh->SetRelativeScale3D(FVector(20, 15, 1));
	}
	if (BoxComponent) {
		BoxComponent->SetRelativeScale3D(FVector(1,25,25));
		BoxComponent->SetRelativeLocation(FVector(1000,0,800));
	}

	if (FrountArrow) {
		FrountArrow->SetRelativeLocation(FVector(1000, 0, 0));
	}

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
	if (OtherActor && OtherActor->IsA(APlayablePlayer::StaticClass()))
	{
		
		ASpace_gameMode* GameMode = Cast<ASpace_gameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (GameMode)
		{
			GameMode->SpawnNextFloor();
		}
	}
}


void AFloor::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->IsA(APlayablePlayer::StaticClass()))
	{
	
		Destroy();
	}
}

FVector AFloor::ArrowLocation()
{

	return FrountArrow->GetComponentLocation();
}
	
	


