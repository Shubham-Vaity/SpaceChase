// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "PlayablePlayer.generated.h"

UCLASS()
class SPACECHASE_API APlayablePlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayablePlayer();

	/** Components */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* Camera;


	// The new root for the mesh (so we can move the mesh only)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* MeshRoot;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
