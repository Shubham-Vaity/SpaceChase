// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "BossType1.generated.h"

UCLASS()
class SPACECHASE_API ABossType1 : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ABossType1();

protected:
	
	virtual void BeginPlay() override;


	
	
	void MoveToNextArrow();  
	void StartMovingMeshToArrow(); 
	void CheckPlayerDistance();
	void UpdateBossSpeed();
	void StartBulletBarrage();
	void FireRandomBullet();
	void StopBulletBarrage();
	void StartSpinAttack();
	void PerformSpinAttack();
	void ReturnInFrontOfPlayer();
	void ResetSpinAttack();
	void ReturnToFrontTick();
	void FinishSpinAttack();


	FVector StartLoc;
	FVector TargetLoc;
	float LerpAlpha = 0.f;
	bool bIsMoving = false;

	FTimerHandle MeshMoveTimer;
	FTimerHandle DelayTimer;
	FTimerHandle ProximityCheckTimer;
	FTimerHandle BossSpeedCheckTimer;
	FTimerHandle BulletFireTimer;
	FTimerHandle StopFiringTimer;
	FTimerHandle SomePhaseTimer;
	FTimerHandle SpinAttackCooldownTimer;
	FTimerHandle ReturnToFrontTimer;
	FTimerHandle SpinAttackLoopTimer;
	FTimerHandle SpinTickTimer ;  // for spin attack
	FTimerHandle ReturnToFrontTickTimer;

	
	AActor* PlayerRef = nullptr;


public:	
	// Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* BossMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProjectileMovementComponent* ProjectileMovement;


	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UArrowComponent* BulletFirePoint;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<AActor> BulletClass;
	
	
	// Arrow movement
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UArrowComponent*> MovementPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveDuration = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WaitTimeMin = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WaitTimeMax = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MeshMoveInterval = 0.01f;

	float ActivationDistance = 3000.f;


	//gpt
	
	float SprintDistanceBuffer = 3000.f;   // Player within this range => boss speeds up
	float MaxSafeDistance = 8000.f;        // Beyond this range => boss stops
	float DefaultBossSpeed = 1000.f;
	float SprintEscapeSpeed = 20000.f;

	float BulletFireInterval = 0.2f;
	float FiringDuration = 2.5f;


	// Attack control
	bool bCanDoSpinAttack = true;
	bool bIsSpinning = false;
	bool bUpdateBossSpeed = true;

	
	// For rotation and movement
	FRotator SpinRotationRate;
	FVector SpinTargetLocation;
	FVector ReturnLocation;
	FRotator OriginalBossRotation;

	
	bool bIsReturningToPlayer = false;

	UPROPERTY(EditAnywhere, Category="Boss|Return")
	float ReturnMoveSpeed = 10000.f;

	FVector SpinStartPosition;
	
	FTimerHandle SpinDurationTimer;
	bool bSpinAttackInProgress = false;
	float  SpinDashSpeed= 50000.0f;
};
