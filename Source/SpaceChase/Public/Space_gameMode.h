#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "Floor.h"
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

    void GetForwardArrowLocation(AFloor* SpawnedFloor);

    FRotator SpawnRotation = FRotator::ZeroRotator;
    FVector ArrowLocation;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floor")
    TSubclassOf<class AFloor> FloorReff;
};
