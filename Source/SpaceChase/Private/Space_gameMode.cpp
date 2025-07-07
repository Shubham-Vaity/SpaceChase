#include "Space_gameMode.h"
#include "Floor.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

ASpace_gameMode::ASpace_gameMode()
{
}

void ASpace_gameMode::BeginPlay()
{
    Super::BeginPlay();

    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController)
    {
        PlayerController->bShowMouseCursor = true;
        PlayerController->bEnableClickEvents = true;
        PlayerController->bEnableMouseOverEvents = true;
    }

    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (!PlayerCharacter)
    {
        UE_LOG(LogTemp, Error, TEXT("PlayerCharacter not found!"));
        return;
    }

    FVector PlayerLocation = PlayerCharacter->GetActorLocation() - FVector(0.f, 0.f, 500.f);

    // Spawn the first floor
    AFloor* SpawnedFloor = nullptr;
    if (StarterFloorReff)
    {
        SpawnedFloor = GetWorld()->SpawnActor<AFloor>(StarterFloorReff, PlayerLocation, SpawnRotation);
        UpdateArrowLocation(SpawnedFloor);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("StarterFloorReff is null!"));
        return;
    }

    // Spawn 10 random floors
    for (int i = 0; i < 8; i++)
    {
        TSubclassOf<AFloor> RandomFloorClass = GetRandomFloor();
        if (RandomFloorClass)
        {
            SpawnedFloor = GetWorld()->SpawnActor<AFloor>(RandomFloorClass, ArrowLocation, SpawnRotation);
            UpdateArrowLocation(SpawnedFloor);
        }
    }
}

void ASpace_gameMode::UpdateArrowLocation(AFloor* SpawnedFloor)
{
    if (SpawnedFloor)
    {
        ArrowLocation = SpawnedFloor->ArrowLocation(); // AFloor must implement this
    }
}


TSubclassOf<AFloor> ASpace_gameMode::GetRandomFloor()
{
    if (FloorTypes.Num() == 0) return nullptr;

    int32 NewIndex;
    
    // Keep generating until it's different from the previous one
    do
    {
        NewIndex = FMath::RandRange(0, FloorTypes.Num() - 1);
    }
    while (FloorTypes.Num() > 1 && NewIndex == LastFloorIndex);

    LastFloorIndex = NewIndex;
    return FloorTypes[NewIndex];
}

void ASpace_gameMode::SpawnNextFloor()
{
    TSubclassOf<AFloor> RandomFloorClass = GetRandomFloor();
    if (!RandomFloorClass) return;

    AFloor* NewFloor = GetWorld()->SpawnActor<AFloor>(RandomFloorClass, ArrowLocation, SpawnRotation);
    if (NewFloor)
    {
        ArrowLocation = NewFloor->ArrowLocation();
        UE_LOG(LogTemp, Warning, TEXT("New floor spawned."));
    }
}
