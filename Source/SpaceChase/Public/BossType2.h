#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossType2.generated.h"

class USkeletalMeshComponent;
class UProjectileMovementComponent;
class UArrowComponent;
class USceneComponent;

UCLASS()
class SPACECHASE_API ABossType2 : public AActor
{
	GENERATED_BODY()
	
public:	
	ABossType2();

protected:
	virtual void BeginPlay() override;

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* BossMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UArrowComponent* BulletFirePoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UArrowComponent*> MovementPoints;

	// Timers
	FTimerHandle MeshMoveTimer;
	FTimerHandle DelayTimer;
	FTimerHandle ProximityCheckTimer;
	FTimerHandle SpeedCheckTimer;
	FTimerHandle RandomFireTimer;
	FTimerHandle DashCheckTimer;
	FTimerHandle DashStartTimer;
	FTimerHandle DashReturnTimer;

	// Movement
	FVector StartLoc;
	FVector TargetLoc;
	float LerpAlpha;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MeshMoveInterval = 0.02f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveDuration = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WaitTimeMin = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WaitTimeMax = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ActivationDistance = 3000.f;

	// Player
	AActor* PlayerRef = nullptr;

	// Bullets
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> BulletClass;

	// Speed logic
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SprintDistanceBuffer = 3000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxSafeDistance = 8000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DefaultBossSpeed = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SprintEscapeSpeed = 8000.f;

	// Dash logic
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DashSpeed = 8000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DashDistanceThreshold = 200.f;

	bool bIsDashing = false;
	bool bCanDash = true;

	FVector DashTargetLocation;
	FVector ReturnToPlayerFrontLocation;

	// Core Behaviors
	void CheckPlayerDistance();
	void UpdateSpeed();
	void MoveToNextArrow();
	void StartMovingMeshToArrow();
	void StartRandomFire();
	void StartDashAttack();
	void DashForward();
	void ReturnToPlayerFront();
	void ResetDashAttack();
	

public:	
	virtual void Tick(float DeltaTime) override;
};
