// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Enemy_Projectile.generated.h"

UCLASS()
class SPACECHASE_API AEnemy_Projectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemy_Projectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FTimerHandle homing;
	FTimerHandle Death;
	

	UPROPERTY( EditAnywhere, BlueprintReadWrite)
	UProjectileMovementComponent * ProjectileMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	URotatingMovementComponent* RotatingMovement;
	
	UPROPERTY( EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMesh;
	
	UPROPERTY( EditAnywhere, BlueprintReadWrite)
	UBoxComponent* BoxCollider;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ACharacter*PlayerReff;

	void home();
	
	 void Die();
	
};
