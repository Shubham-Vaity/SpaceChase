// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Bullet.h"
#include "Components/ArrowComponent.h"

#include "EnemyType2.generated.h"

UCLASS()
class SPACECHASE_API AEnemyType2 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyType2();





protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	FTimerHandle DelayTimerHandle;
	FTimerHandle StartShouting;
	
	FTimerHandle death;


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

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool Shoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ACharacter*PlayerReff;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AActor> EnemyBullet;


	//functions
	UFUNCTION()
	void GoToLocation();

	UFUNCTION()
	void ShoutPlayer();


	UFUNCTION(BlueprintInternalUseOnlyHierarchical)
	void Die();
	
 	void DestroyEnemy();


	
};
