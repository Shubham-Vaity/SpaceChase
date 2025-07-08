// BossType1.cpp - Fully updated to use USkeletalMeshComponent and arrow-based random movement

#include "BossType1.h"
#include "PlayablePlayer.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SkeletalMeshComponent.h"

ABossType1::ABossType1()
{
	PrimaryActorTick.bCanEverTick = false; // no Tick needed

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	BossMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BossMesh"));

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 0.0f;
	ProjectileMovement->MaxSpeed = 600.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;

	BulletFirePoint= CreateDefaultSubobject<UArrowComponent>(TEXT("BulletFirePoint"));

	

	BossMesh->SetupAttachment(Root);
	BulletFirePoint->SetupAttachment(BossMesh);
	RootComponent = Root;

	
}

void ABossType1::BeginPlay()
{
	Super::BeginPlay();

	PlayerRef = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);


	GetWorld()->GetTimerManager().SetTimer(BossSpeedCheckTimer, this, &ABossType1::UpdateBossSpeed, 0.01f, true);

	GetWorld()->GetTimerManager().SetTimer(ProximityCheckTimer, this, &ABossType1::CheckPlayerDistance, 0.2f, true);
	
	GetWorld()->GetTimerManager().SetTimer(SomePhaseTimer, this, &ABossType1::StartBulletBarrage, 5.f, true);
	
	GetWorld()->GetTimerManager().SetTimer(SpinAttackLoopTimer, this, &ABossType1::StartSpinAttack, 10.f, false);
}

void ABossType1::CheckPlayerDistance()
{
	if (!PlayerRef || !BossMesh) return;

	float Distance = FVector::Dist(PlayerRef->GetActorLocation(), GetActorLocation());

	if (Distance < ActivationDistance) // Set this in constructor or header (e.g., 3000.f)
	{
		// Start boss movement
		GetWorld()->GetTimerManager().ClearTimer(ProximityCheckTimer);

		if (ProjectileMovement)
		{
			ProjectileMovement->Velocity = FVector(600.f, 0.f, 0.f);
		}

		MoveToNextArrow();
	}
}


void ABossType1::UpdateBossSpeed()
{
	if (!bUpdateBossSpeed || !PlayerRef || !ProjectileMovement) return;

	float Distance = FVector::Dist(PlayerRef->GetActorLocation(), GetActorLocation());

	if (Distance < SprintDistanceBuffer)
	{
		// Player is too close – boss speeds up
		ProjectileMovement->Velocity = FVector(SprintEscapeSpeed, 0.f, 0.f);
	}
	else if (Distance > MaxSafeDistance)
	{
		// Player is too far – boss stops
		ProjectileMovement->Velocity = FVector(0.f, 0.f, 0.f);
	}
	else
	{
		// Normal distance – boss moves at normal pace
		ProjectileMovement->Velocity = FVector(DefaultBossSpeed, 0.f, 0.f);
	}
}


void ABossType1::MoveToNextArrow()
{
	if (MovementPoints.Num() == 0) return;
	ProjectileMovement->Velocity = FVector(4000.f, 0.f, 0.f);
	int32 Index = FMath::RandRange(0, MovementPoints.Num() - 1);
	TargetLoc = MovementPoints[Index]->GetComponentLocation();
	StartLoc = BossMesh->GetComponentLocation();
	LerpAlpha = 0.f;
	bIsMoving = true;

	GetWorld()->GetTimerManager().SetTimer(MeshMoveTimer, this, &ABossType1::StartMovingMeshToArrow, MeshMoveInterval, true);
}

void ABossType1::StartMovingMeshToArrow()
{
	LerpAlpha += MeshMoveInterval / MoveDuration;

	FVector NewLocation = FMath::Lerp(StartLoc, TargetLoc, LerpAlpha);
	NewLocation.X = BossMesh->GetComponentLocation().X; // keep X steady
	BossMesh->SetWorldLocation(NewLocation);

	if (LerpAlpha >= 1.f)
	{
		GetWorld()->GetTimerManager().ClearTimer(MeshMoveTimer);

		float Wait = FMath::FRandRange(WaitTimeMin, WaitTimeMax);
		GetWorld()->GetTimerManager().SetTimer(DelayTimer, this, &ABossType1::MoveToNextArrow, Wait, false);
	}
}

void ABossType1::StartBulletBarrage()
{
	// Stop the mesh movement temporarily
	GetWorld()->GetTimerManager().ClearTimer(MeshMoveTimer);

	// Start firing bullets every 0.2 seconds
	GetWorld()->GetTimerManager().SetTimer(BulletFireTimer, this, &ABossType1::FireRandomBullet, BulletFireInterval, true);

	// Stop firing after FiringDuration
	GetWorld()->GetTimerManager().SetTimer(StopFiringTimer, this, &ABossType1::StopBulletBarrage, FiringDuration, false);
}

void ABossType1::FireRandomBullet()
{
	if (!BulletClass || !BulletFirePoint) return;

	FVector SpawnLoc = BulletFirePoint->GetComponentLocation();
	FRotator BaseRot = BulletFirePoint->GetComponentRotation();

	// Random rotation within a forward cone (e.g., ±30° in yaw and ±15° in pitch)
	float RandYaw = FMath::FRandRange(-30.f, 30.f);
	float RandPitch = FMath::FRandRange(-15.f, 15.f);
	FRotator RandomRot = BaseRot.Add(RandPitch, RandYaw, 0.f);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GetWorld()->SpawnActor<AActor>(BulletClass, SpawnLoc, RandomRot, SpawnParams);
}

void ABossType1::StopBulletBarrage()
{
	GetWorld()->GetTimerManager().ClearTimer(BulletFireTimer);

	// Resume movement to next arrow after firing
	MoveToNextArrow();
}
void ABossType1::StartSpinAttack()
{
	if (!bCanDoSpinAttack || !PlayerRef || bSpinAttackInProgress) return;

	bSpinAttackInProgress = true;
	bIsSpinning = true;
	bCanDoSpinAttack = false;
	bUpdateBossSpeed = false;
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this); // Clear all other logic

	OriginalBossRotation = BossMesh->GetComponentRotation();
	SpinStartPosition = GetActorLocation();

	
	// Spin rate
	SpinRotationRate = FRotator(0.f,  1440.f, 0.f);

	// Start spinning movement tick
	GetWorld()->GetTimerManager().SetTimer(MeshMoveTimer, this, &ABossType1::PerformSpinAttack, MeshMoveInterval, true);

	// Start 5-second duration timer
	GetWorld()->GetTimerManager().SetTimer(SpinDurationTimer, this, &ABossType1::FinishSpinAttack, 5.0f, false);

	// Initial dash toward player
	SpinTargetLocation = PlayerRef->GetActorLocation();
	ProjectileMovement->Velocity = (SpinTargetLocation - GetActorLocation()).GetSafeNormal() * SpinDashSpeed;
}
void ABossType1::PerformSpinAttack()
{
	if (!bIsSpinning || !BossMesh) return;

	FRotator NewRotation = BossMesh->GetComponentRotation() + (SpinRotationRate * MeshMoveInterval);
	BossMesh->SetWorldRotation(NewRotation);
}


void ABossType1::ReturnInFrontOfPlayer()
{
	bIsReturningToPlayer = true;
	GetWorld()->GetTimerManager().SetTimer(ReturnToFrontTickTimer, this, &ABossType1::ReturnToFrontTick, 0.01f, true);

}

void ABossType1::ResetSpinAttack()
{
	if (!IsValid(this) || !IsValid(PlayerRef)) return;
	bCanDoSpinAttack = true;
	bUpdateBossSpeed = true; // Re-enable if needed after attack

	StartSpinAttack();
}
void ABossType1::ReturnToFrontTick()
{
	if (!PlayerRef || !bIsReturningToPlayer) return;

	FVector Direction = (SpinStartPosition - GetActorLocation()).GetSafeNormal();
	if (!Direction.IsNearlyZero())
	{
		ProjectileMovement->Velocity = Direction * ReturnMoveSpeed;
	}

	float Distance = FVector::DistXY(GetActorLocation(), SpinStartPosition);
	if (Distance < 100.f)
	{
		bIsReturningToPlayer = false;
		ProjectileMovement->Velocity = FVector::ZeroVector;

		// Restore original rotation
		BossMesh->SetWorldRotation(OriginalBossRotation);
		GetWorld()->GetTimerManager().ClearTimer(ReturnToFrontTickTimer);

		// Re-enable all logic
		bCanDoSpinAttack = true;
		bUpdateBossSpeed = true;
		bSpinAttackInProgress = false;

		// Restart timers
		GetWorld()->GetTimerManager().SetTimer(BossSpeedCheckTimer, this, &ABossType1::UpdateBossSpeed, 0.05f, true);
		GetWorld()->GetTimerManager().SetTimer(SomePhaseTimer, this, &ABossType1::StartBulletBarrage, 5.f, true);
		GetWorld()->GetTimerManager().SetTimer(SpinAttackCooldownTimer, this, &ABossType1::StartSpinAttack, 10.f, true);
	}
}


void ABossType1::FinishSpinAttack()
{
	bIsSpinning = false;
	GetWorld()->GetTimerManager().ClearTimer(MeshMoveTimer);

	// Stop dash movement
	ProjectileMovement->Velocity = FVector::ZeroVector;

	// Start returning to the original spin start position
	bIsReturningToPlayer = true;
	GetWorld()->GetTimerManager().SetTimer(ReturnToFrontTickTimer, this, &ABossType1::ReturnToFrontTick, 0.02f, true);
}
