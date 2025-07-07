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

protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floor Spawning")
    TSubclassOf<AFloor> StarterFloorReff;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floor Spawning")
    TArray<TSubclassOf<AFloor>> FloorTypes;

    void SpawnNextFloor();
    TSubclassOf<AFloor> GetRandomFloor();

private:
    FVector ArrowLocation;
    FRotator SpawnRotation = FRotator::ZeroRotator;

    int32 LastFloorIndex = -1;
    void UpdateArrowLocation(AFloor* SpawnedFloor);
};
