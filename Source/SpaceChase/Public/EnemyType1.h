// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/ArrowComponent.h"	
#include "GameFramework/ProjectileMovementComponent.h"
#include "EnemyType1.generated.h"

UCLASS()
class SPACECHASE_API AEnemyType1 : public AActor
{
	GENERATED_BODY()
	
public:	

	AEnemyType1();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle DelayTimerHandle;
	FTimerHandle StopHomingHandle;


	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	//components
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* DefaultSceneRoot;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* SkeletalMesh;	


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* hitBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UArrowComponent* FireSpot;


	
	//Variables
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool Run;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ACharacter*PlayerReff;


	//functions
	UFUNCTION()
	void GoToLocation();

	UFUNCTION()
	void ShoutPlayer();

	UFUNCTION()
	void StopHomingNow();
	
};
