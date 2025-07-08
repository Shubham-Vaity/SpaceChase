#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Space_gameMode.generated.h"

class AFloor;

UCLASS()
class SPACECHASE_API ASpace_gameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ASpace_gameMode();

    virtual void BeginPlay() override;

    void SpawnNextFloor();
    void UpdateArrowLocation(AFloor* SpawnedFloor);
    // Randomly chosen array for each biome
	UPROPERTY(EditAnywhere,BlueprintreadWrite)
    TArray<TSubclassOf<AFloor>> Biome1Array;

	UPROPERTY(EditAnywhere,BlueprintreadWrite)
    TArray<TSubclassOf<AFloor>> Biome2Array;

	UPROPERTY(EditAnywhere,BlueprintreadWrite)
    TArray<TSubclassOf<AFloor>> Biome3Array;

   UFUNCTION(BlueprintCallable)
    void SetActiveFloorArray();

    UFUNCTION(BlueprintCallable)
    void OnBossDefeated();

    
   TSubclassOf<AFloor> GetRandomFloor();

    
protected:
    // Floor arrays
    UPROPERTY(EditAnywhere,BlueprintreadWrite)
    TArray<TSubclassOf<AFloor>> FloorBiome1Array;

    UPROPERTY(EditAnywhere,BlueprintreadWrite)
    TArray<TSubclassOf<AFloor>> FloorBiome2Array;

    UPROPERTY(EditAnywhere,BlueprintreadWrite)
    TArray<TSubclassOf<AFloor>> FloorBiome3Array;

    UPROPERTY(EditAnywhere,BlueprintreadWrite)
    TArray<TSubclassOf<AFloor>> FloorBiome4Array;

    UPROPERTY(EditAnywhere,BlueprintreadWrite)
    TArray<TSubclassOf<AFloor>> FloorBiome5Array;

    UPROPERTY(EditAnywhere,BlueprintreadWrite)
    TArray<TSubclassOf<AFloor>> FloorBiome6Array;

    UPROPERTY(EditAnywhere,BlueprintreadWrite)
    TArray<TSubclassOf<AFloor>> DefaultFloors;

    // Bosses
    UPROPERTY(EditAnywhere,BlueprintreadWrite)
    TSubclassOf<AActor> Boss1;

    UPROPERTY(EditAnywhere,BlueprintreadWrite)
    TSubclassOf<AActor> Boss2;

    UPROPERTY(EditAnywhere,BlueprintreadWrite)
    TSubclassOf<AActor> Boss3;

    // Starter
    UPROPERTY(EditAnywhere,BlueprintreadWrite)
    TSubclassOf<AFloor> StarterFloorReff;

    // Floor Management
    int32 LastFloorIndex = -1;
    UPROPERTY(EditAnywhere,BlueprintreadWrite)
    int32 FloorCounter = 0;
    UPROPERTY(EditAnywhere,BlueprintreadWrite)
    int32 CurrentBiome = 1;

    UPROPERTY(EditAnywhere,BlueprintreadWrite)
    bool bBossAlive = false;
    bool bBiomeCompleted = false;

    UPROPERTY(EditAnywhere,BlueprintreadWrite)
    int32 MaxFloorsPerBiome = 20;

    FVector ArrowLocation;
    FRotator SpawnRotation = FRotator::ZeroRotator;

    TArray<TSubclassOf<AFloor>> ActiveFloorArray;
    
  
    void TrySpawnBoss();
};
