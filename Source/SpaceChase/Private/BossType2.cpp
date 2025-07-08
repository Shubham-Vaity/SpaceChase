#include "BossType2.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SkeletalMeshComponent.h"

ABossType2::ABossType2()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	BossMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BossMesh"));
	BossMesh->SetupAttachment(RootComponent);
	BossMesh->SetRelativeRotation(FRotator(0.f, 0.f, -90.f)); // Face forward

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 0.f;
	ProjectileMovement->MaxSpeed = 1000.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;

	BulletFirePoint = CreateDefaultSubobject<UArrowComponent>(TEXT("BulletFirePoint"));
	BulletFirePoint->SetupAttachment(BossMesh);
}

void ABossType2::BeginPlay()
{
	Super::BeginPlay();
	PlayerRef = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	GetWorld()->GetTimerManager().SetTimer(ProximityCheckTimer, this, &ABossType2::CheckPlayerDistance, 0.2f, true);
	GetWorld()->GetTimerManager().SetTimer(SpeedCheckTimer, this, &ABossType2::UpdateSpeed, 0.05f, true);
	GetWorld()->GetTimerManager().SetTimer(RandomFireTimer, this, &ABossType2::StartRandomFire, 1.5f, true);
	GetWorld()->GetTimerManager().SetTimer(DashCheckTimer, this, &ABossType2::StartDashAttack, 7.f, true);
}

void ABossType2::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

void ABossType2::CheckPlayerDistance()
{
	if (!PlayerRef) return;

	float Distance = FVector::Dist(GetActorLocation(), PlayerRef->GetActorLocation());
	if (Distance < ActivationDistance)
	{
		GetWorld()->GetTimerManager().ClearTimer(ProximityCheckTimer);
		ProjectileMovement->Velocity = FVector(DefaultBossSpeed, 0.f, 0.f);
		MoveToNextArrow();
	}
}

void ABossType2::UpdateSpeed()
{
	if (!PlayerRef || bIsDashing) return;

	float Distance = FVector::Dist(PlayerRef->GetActorLocation(), GetActorLocation());

	if (Distance < SprintDistanceBuffer)
	{
		ProjectileMovement->Velocity = FVector(SprintEscapeSpeed, 0.f, 0.f);
	}
	else if (Distance > MaxSafeDistance)
	{
		ProjectileMovement->Velocity = FVector::ZeroVector;
	}
	else
	{
		ProjectileMovement->Velocity = FVector(DefaultBossSpeed, 0.f, 0.f);
	}

	// Face player
	FVector ToPlayer = PlayerRef->GetActorLocation() - GetActorLocation();
	FRotator LookRotation = FRotationMatrix::MakeFromX(ToPlayer).Rotator();
	BossMesh->SetWorldRotation(LookRotation + FRotator(0.f, -90.f, 0.f));
}

void ABossType2::MoveToNextArrow()
{
	if (MovementPoints.Num() == 0 || bIsDashing) return;

	int32 Index = FMath::RandRange(0, MovementPoints.Num() - 1);
	TargetLoc = MovementPoints[Index]->GetComponentLocation();
	StartLoc = BossMesh->GetComponentLocation();
	LerpAlpha = 0.f;

	GetWorld()->GetTimerManager().SetTimer(MeshMoveTimer, this, &ABossType2::StartMovingMeshToArrow, MeshMoveInterval, true);
}

void ABossType2::StartMovingMeshToArrow()
{
	LerpAlpha += MeshMoveInterval / MoveDuration;
	FVector NewLocation = FMath::Lerp(StartLoc, TargetLoc, LerpAlpha);
	NewLocation.X = BossMesh->GetComponentLocation().X;
	BossMesh->SetWorldLocation(NewLocation);

	if (LerpAlpha >= 1.f)
	{
		GetWorld()->GetTimerManager().ClearTimer(MeshMoveTimer);
		float Wait = FMath::FRandRange(WaitTimeMin, WaitTimeMax);
		GetWorld()->GetTimerManager().SetTimer(DelayTimer, this, &ABossType2::MoveToNextArrow, Wait, false);
	}
}

void ABossType2::StartRandomFire()
{
	if (!BulletClass || bIsDashing) return;

	FVector SpawnLoc = BulletFirePoint->GetComponentLocation();
	FRotator BaseRot = BulletFirePoint->GetComponentRotation();
	float RandYaw = FMath::FRandRange(-20.f, 20.f);
	float RandPitch = FMath::FRandRange(-10.f, 10.f);
	FRotator FireRot = BaseRot.Add(RandPitch, RandYaw, 0.f);

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetWorld()->SpawnActor<AActor>(BulletClass, SpawnLoc, FireRot, Params);
}

void ABossType2::StartDashAttack()
{
	if (!bCanDash || !PlayerRef || bIsDashing) return;

	bIsDashing = true;
	bCanDash = false;

	DashTargetLocation = PlayerRef->GetActorLocation();
	FVector Direction = (DashTargetLocation - GetActorLocation()).GetSafeNormal();

	ProjectileMovement->Velocity = Direction * DashSpeed;
	BossMesh->SetWorldRotation(Direction.Rotation() + FRotator(0.f, -90.f, 0.f));

	GetWorld()->GetTimerManager().SetTimer(DashStartTimer, this, &ABossType2::DashForward, 0.01f, true);
}

void ABossType2::DashForward()
{
	if (!PlayerRef || !bIsDashing) return;

	float Distance = FVector::DistXY(GetActorLocation(), DashTargetLocation);

	if (Distance < DashDistanceThreshold)
	{
		GetWorld()->GetTimerManager().ClearTimer(DashStartTimer);

		FVector ForwardDir = GetActorForwardVector();
		ProjectileMovement->Velocity = ForwardDir * DashSpeed;

		GetWorld()->GetTimerManager().SetTimer(DashReturnTimer, this, &ABossType2::ReturnToPlayerFront, 0.7f, false);
	}
}

void ABossType2::ReturnToPlayerFront()
{
	if (!PlayerRef) return;

	ReturnToPlayerFrontLocation = PlayerRef->GetActorLocation() + FVector(500.f, 0.f, 0.f);
	SetActorLocation(ReturnToPlayerFrontLocation);

	FVector ToPlayer = PlayerRef->GetActorLocation() - GetActorLocation();
	FRotator LookAtRot = FRotationMatrix::MakeFromX(ToPlayer).Rotator();
	BossMesh->SetWorldRotation(LookAtRot + FRotator(0.f, -90.f, 0.f));

	ResetDashAttack();
}

void ABossType2::ResetDashAttack()
{
	bIsDashing = false;
	bCanDash = true;

	MoveToNextArrow();
}
