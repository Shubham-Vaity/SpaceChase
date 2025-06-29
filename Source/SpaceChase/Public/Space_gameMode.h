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
    TArray<TSubclassOf<class AFloor>> FloorTypes;

    void SpawnNextFloor();

    
    

private:
    FVector ArrowLocation;
    FRotator SpawnRotation = FRotator::ZeroRotator; // Or initialize as needed
    void UpdateArrowLocation(AFloor* SpawnedFloor);


};
