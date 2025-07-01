// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Bullet.h"
#include "PlayablePlayer.h" 
#include "Components/ArrowComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyType3.generated.h"

UCLASS()
class SPACECHASE_API AEnemyType3 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyType3();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle DelayTimerHandle;
	FTimerHandle StartShouting;
	
	


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
	bool Shoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	APlayablePlayer*PlayerReff;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AActor> EnemyBullet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Poss;

	//functions
	UFUNCTION()
	void DoPattern();

	UFUNCTION(BlueprintImplementableEvent)
	void GetPattern(int Possition);
	
	UFUNCTION()
	void ShoutPlayer();

	UFUNCTION()
	void Die(); 	
 		
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
