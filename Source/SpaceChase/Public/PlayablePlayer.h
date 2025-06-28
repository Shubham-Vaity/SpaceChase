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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	class UCameraComponent* Camera;


	// The new root for the mesh (so we can move the mesh only)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* MeshRoot;

	//Gun
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UArrowComponent* FirePoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ABullet> Bullet1;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//test


	// variables
	FRotator TargetArmRotation;
	FRotator CurrentArmRotation;
	float CameraTiltSpeed = 5.0f;
	float MaxTiltAngle = 10.0f;  // How much the camera tilts

	float LastInputY = 0.0f;
	float LastInputZ = 0.0f;


	//functions
	void MoveForward(float Value);
	void MoveRight(float Value);
	void FireAtMouse();

//GPT
	// Movement bounds
	float MinY = -200.f;
	float MaxY = 200.f;
	float MinZ = -100.f;
	float MaxZ = 300.f;

	// Input trackers
	float InputY;
	float InputZ;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

}; 