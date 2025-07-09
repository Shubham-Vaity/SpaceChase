// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/StaticMeshComponent.h"
#include "CoreMinimal.h"
#include "Space_gameMode.h"
#include "Components/SceneComponent.h"  
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Floor.generated.h"

UCLASS()
class SPACECHASE_API AFloor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloor();

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	FTimerHandle DelayTimerHandle;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//components
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* MyStaticMesh;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* DefaultSceneRoot;



	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UArrowComponent* FrountArrow;



	//functions

	UFUNCTION()
	void spawnEnemyAtArrows();

	UFUNCTION()
	FVector ArrowLocation();
	
	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);



	//enemy spawner
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "enemy spawner")
	TArray<UArrowComponent*> SpawnArrows;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<AActor>> EnemyRefs;


	// Tracks if enemies have been spawned
	bool bEnemiesSpawned = false;

	// Timer handle for repeated spawn check
	FTimerHandle EnemySpawnCheckTimer;
};
