#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Space_gameMode.generated.h"

UCLASS()
class SPACECHASE_API ASpace_gameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ASpace_gameMode();

protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<class AFloor> FloorReff;

    void GetForwardArrowLocation(class AFloor* SpawnedFloor);
    void SpawnNextFloor();
    AFloor* NewFloor; // optional: remove if only used locally in function

private:
    FVector ArrowLocation;
    FRotator SpawnRotation = FRotator::ZeroRotator; // Or initialize as needed

};
