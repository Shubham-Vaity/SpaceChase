#include "Space_gameMode.h"
#include "Floor.h"
#include "PlayablePlayer.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

ASpace_gameMode::ASpace_gameMode()
{
}

void ASpace_gameMode::BeginPlay()
{
    Super::BeginPlay();

    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (PlayerCharacter && FloorReff)
    {
        FVector PlayerLocation = PlayerCharacter->GetActorLocation() - FVector(0.f, 0.f, 500.f);
        

        AFloor* SpawnedFloor = GetWorld()->SpawnActor<AFloor>(FloorReff, PlayerLocation, SpawnRotation);
        GetForwardArrowLocation(SpawnedFloor);

        for (int i = 0; i < 10; i++)
        {
            SpawnedFloor = GetWorld()->SpawnActor<AFloor>(FloorReff, ArrowLocation, SpawnRotation);
            GetForwardArrowLocation(SpawnedFloor);
        }

        UE_LOG(LogTemp, Warning, TEXT("GameMode: Spawned initial floors."));
    }
}

void ASpace_gameMode::GetForwardArrowLocation(AFloor* SpawnedFloor)
{
    if (SpawnedFloor && SpawnedFloor->FrountArrow)
    {
        ArrowLocation = SpawnedFloor->FrountArrow->GetComponentLocation();
        
    }
}
