// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/ArrowComponent.h"
#include "Bullet.h"
#include "PlayablePlayer.generated.h"

UCLASS()
class SPACECHASE_API APlayablePlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayablePlayer();

	/** Components */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	class UCameraComponent* Camera;


	// The new root for the mesh (so we can move the mesh only)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* MeshRoot;

	//Gun
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UArrowComponent* FirePoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ABullet> Bullet1;

	//enemyPoint
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UArrowComponent* EnemType3;


	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//test


	
	// variabless
	FRotator TargetArmRotation;
	FRotator CurrentArmRotation;
	float CameraTiltSpeed = 5.0f;
	float MaxTiltAngle = 10.0f;  // How much the camera tilts

	float LastInputY = 0.0f;
	float LastInputZ = 0.0f;

	float Speed=500.0f;


	//functions
	void MoveForward(float Value);
	void MoveRight(float Value);
	void FireAtMouse();
	void increaseTimer();

//gpt

	FVector InitialCapsuleLocation;
	FVector InitialMeshOffset;

	UPROPERTY(EditAnywhere, Category="Movement Limits")
	float MaxMeshOffsetY = 250.f;

	UPROPERTY(EditAnywhere, Category="Movement Limits")
	float MaxMeshOffsetZ = 250.f;

	UPROPERTY(EditAnywhere, Category="Movement Limits")
	float MaxCapsuleOffsetY = 300.f;

	UPROPERTY(EditAnywhere, Category="Movement Limits")
	float MaxCapsuleOffsetZ = 300.f;

	UPROPERTY(EditAnywhere, Category="Movement Speed")
	float MeshMoveSpeed = 8.f;

	UPROPERTY(EditAnywhere, Category="Movement Speed")
	float CapsuleMoveSpeed = 6.f;

	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// --- Sprinting ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsSprinting;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SprintSpeed = 1500.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float NormalSpeed = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Stamina = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxStamina = 100.f;
	
	float StaminaDrainRate = 20.f;  // per second
	float StaminaRecoveryRate = 10.f; // per second

	float SprintTimer = 0.f;
	float MaxSprintTime = 5.f;

	void StartSprint();
	void StopSprint();
	float StaminaTickRate = 0.1f;
	UFUNCTION()
	void UpdateStamina();

	FTimerHandle DelayTimerHandle;
	


	
}; 