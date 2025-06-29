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


    //show cursor
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

    if (PlayerController)
    {
        PlayerController->bShowMouseCursor = true;
        PlayerController->bEnableClickEvents = true;
        PlayerController->bEnableMouseOverEvents = true;
    }




    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (PlayerCharacter && FloorTypes.Num() > 0)
    {
        FVector PlayerLocation = PlayerCharacter->GetActorLocation() - FVector(0.f, 0.f, 500.f);

        int32 Index = FMath::RandRange(0, FloorTypes.Num() - 1);
        TSubclassOf<AFloor> FloorToSpawn = FloorTypes[Index];

        AFloor* SpawnedFloor = GetWorld()->SpawnActor<AFloor>(FloorToSpawn, PlayerLocation, SpawnRotation);
        UpdateArrowLocation(SpawnedFloor);

        for (int i = 0; i < 10; i++)
        {
            Index = FMath::RandRange(0, FloorTypes.Num() - 1);
            FloorToSpawn = FloorTypes[Index];
            SpawnedFloor = GetWorld()->SpawnActor<AFloor>(FloorToSpawn, ArrowLocation, SpawnRotation);
            UpdateArrowLocation(SpawnedFloor);
        }

        UE_LOG(LogTemp, Warning, TEXT("GameMode: Spawned initial floors."));
    }
}

void ASpace_gameMode::UpdateArrowLocation(AFloor* SpawnedFloor)
{
    if (SpawnedFloor)
    {
        ArrowLocation = SpawnedFloor->ArrowLocation();
    }
}


void ASpace_gameMode::SpawnNextFloor()
{
    if (FloorTypes.Num() == 0) return;

    int32 Index = FMath::RandRange(0, FloorTypes.Num() - 1);
    TSubclassOf<AFloor> FloorToSpawn = FloorTypes[Index];

    AFloor* NewFloor = GetWorld()->SpawnActor<AFloor>(FloorToSpawn, ArrowLocation, SpawnRotation);
    if (NewFloor)
    {
        ArrowLocation = NewFloor->ArrowLocation();
        UE_LOG(LogTemp, Warning, TEXT("New floor spawned at ArrowLocation"));
    }
}
