#include "Space_gameMode.h"
#include "Floor.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

ASpace_gameMode::ASpace_gameMode()
{
    PrimaryActorTick.bCanEverTick = false;
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

    // Spawn starter floor
    if (StarterFloorReff)
    {
        AFloor* StarterFloor = GetWorld()->SpawnActor<AFloor>(StarterFloorReff, PlayerLocation, SpawnRotation);
        UpdateArrowLocation(StarterFloor);
    }

    // ✅ Randomly assign arrays to biomes (do this before setting active floor array)
    int32 RandIndex1 = FMath::RandBool() ? 0 : 1;
    Biome1Array = (RandIndex1 == 0) ? FloorBiome1Array : FloorBiome2Array;

    int32 RandIndex2 = FMath::RandBool() ? 0 : 1;
    Biome2Array = (RandIndex2 == 0) ? FloorBiome3Array : FloorBiome4Array;

    int32 RandIndex3 = FMath::RandBool() ? 0 : 1;
    Biome3Array = (RandIndex3 == 0) ? FloorBiome5Array : FloorBiome6Array;

    // ✅ Assign first biome's active array
    SetActiveFloorArray();

    // ✅ Spawn 10 floors from biome 1 (these don't count toward FloorCounter)
    for (int i = 0; i < 10; i++)
    {
        TSubclassOf<AFloor> InitialFloor = nullptr;
        if (ActiveFloorArray.Num() == 0)
        {
            UE_LOG(LogTemp, Error, TEXT("Biome array is empty during initial spawn."));
            continue;
        }

        int32 Index = FMath::RandRange(0, ActiveFloorArray.Num() - 1);
        InitialFloor = ActiveFloorArray[Index];

        if (InitialFloor)
        {
            AFloor* Floor = GetWorld()->SpawnActor<AFloor>(InitialFloor, ArrowLocation, SpawnRotation);
            UpdateArrowLocation(Floor);
        }
    }

    // ✅ Then begin one-by-one spawning (now FloorCounter will start counting)
    FloorCounter = 0;
    SpawnNextFloor();
}



void ASpace_gameMode::SetActiveFloorArray()
{
    if (CurrentBiome == 1)
    {
        ActiveFloorArray = Biome1Array;
    }
    else if (CurrentBiome == 2)
    {
        ActiveFloorArray = Biome2Array;
    }
    else if (CurrentBiome == 3)
    {
        ActiveFloorArray = Biome3Array;
    }
    else
    {
        ActiveFloorArray = DefaultFloors;
    }

    FloorCounter = 0;
}

void ASpace_gameMode::SpawnNextFloor()
{
    // ✅ After boss dies and biome is completed, move to next biome
    if (bBiomeCompleted && !bBossAlive)
    {
        CurrentBiome++;
        bBiomeCompleted = false;
        FloorCounter = 0;
        SetActiveFloorArray();
    }

    TSubclassOf<AFloor> FloorToSpawn = nullptr;

    // 1. BIOME FLOOR PHASE
    if (FloorCounter < MaxFloorsPerBiome && !bBiomeCompleted)
    {
        if (ActiveFloorArray.Num() == 0)
        {
            UE_LOG(LogTemp, Error, TEXT("ActiveFloorArray is empty!"));
            return;
        }

        int32 Index;
        do
        {
            Index = FMath::RandRange(0, ActiveFloorArray.Num() - 1);
        } while (Index == LastFloorIndex && ActiveFloorArray.Num() > 1);

        LastFloorIndex = Index;
        FloorToSpawn = ActiveFloorArray[Index];
        FloorCounter++;

        if (FloorCounter >= MaxFloorsPerBiome)
        {
            bBiomeCompleted = true;
            TrySpawnBoss();
        }
    }
    // 2. POST-BIOME DEFAULT PHASE (BOSS PHASE)
    else
    {
        if (DefaultFloors.Num() == 0)
        {
            UE_LOG(LogTemp, Error, TEXT("DefaultFloors is empty!"));
            return;
        }

        int32 DefaultIndex;
        do
        {
            DefaultIndex = FMath::RandRange(0, DefaultFloors.Num() - 1);
        } while (DefaultIndex == LastFloorIndex && DefaultFloors.Num() > 1);

        LastFloorIndex = DefaultIndex;
        FloorToSpawn = DefaultFloors[DefaultIndex];
    }

    // Spawn the chosen floor
    if (FloorToSpawn)
    {
        AFloor* SpawnedFloor = GetWorld()->SpawnActor<AFloor>(FloorToSpawn, ArrowLocation, SpawnRotation);
        UpdateArrowLocation(SpawnedFloor);
    }
}

void ASpace_gameMode::TrySpawnBoss()
{
    if (bBossAlive) return;

    TSubclassOf<AActor> BossToSpawn = nullptr;

    switch (CurrentBiome)
    {
        case 1: BossToSpawn = Boss1; break;
        case 2: BossToSpawn = Boss2; break;
        case 3: BossToSpawn = Boss3; break;
        default: return;
    }

    if (BossToSpawn)
    {
        FVector BossSpawnLoc = ArrowLocation + FVector(300.f, 0.f, 200.f);
        GetWorld()->SpawnActor<AActor>(BossToSpawn, BossSpawnLoc, SpawnRotation);
        bBossAlive = true;
    }
}

void ASpace_gameMode::UpdateArrowLocation(AFloor* SpawnedFloor)
{
    if (SpawnedFloor)
    {
        ArrowLocation = SpawnedFloor->ArrowLocation();
    }
}

void ASpace_gameMode::OnBossDefeated()
{
    bBossAlive = false;
    // Do NOT increment biome here — it will happen in next floor spawn
}
