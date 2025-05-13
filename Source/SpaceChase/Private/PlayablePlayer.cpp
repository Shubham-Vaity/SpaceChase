// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayablePlayer.h"



#include "GameFramework/CharacterMovementComponent.h"  // This is the correct include for UCharacterMovementComponent
#include "GameFramework/SpringArmComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"


// Sets default values
APlayablePlayer::APlayablePlayer()
{

    MeshRoot = CreateDefaultSubobject<USceneComponent>(TEXT("MeshRoot"));
    MeshRoot->SetupAttachment(RootComponent);


    // Attach mesh to MeshRoot
    GetMesh()->SetupAttachment(MeshRoot);
    GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));


    // Set this character to call Tick() every frame. You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;






    // Create Spring Arm
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(RootComponent);
    SpringArm->TargetArmLength = 300.f;

    SpringArm->bUsePawnControlRotation = false;

    // Create Camera
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

    Camera->bUsePawnControlRotation = false;

    // Access the Character Movement Component directly
    UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();  // Using GetCharacterMovement() to access the component

    if (CharacterMovementComponent)
    {
        CharacterMovementComponent->SetMovementMode(MOVE_Flying);
        CharacterMovementComponent->GravityScale = 0.0f;
        CharacterMovementComponent->MaxFlySpeed = 800.0f; // Default is 600

    }


    // Assign the skeletal mesh
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Game/Ship/Object_4.Object_4"));
    if (MeshAsset.Succeeded())
    {
        GetMesh()->SetSkeletalMesh(MeshAsset.Object);
        GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
        GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
        GetMesh()->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));



    }

    // Assign the animation blueprint
    static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBP(TEXT("/Game/Ship/Animation12345.Animation12345"));
    if (AnimBP.Succeeded())
    {
        GetMesh()->SetAnimInstanceClass(AnimBP.Class);
    }

}

// Called when the game starts or when spawned
void APlayablePlayer::BeginPlay()
{
    Super::BeginPlay();


    GetCharacterMovement()->SetMovementMode(MOVE_Flying);



}

// Called every frame
void APlayablePlayer::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    FVector NewLocation = GetActorLocation();
    NewLocation.X += 300.f * DeltaTime; // Adjust speed as needed
    SetActorLocation(NewLocation);


    // --- CAMERA TILT LOGIC ---
    float TiltY = FMath::Clamp(LastInputY, -1.0f, 1.0f) * MaxTiltAngle;  // Side-to-side
    float TiltZ = FMath::Clamp(LastInputZ, -1.0f, 1.0f) * MaxTiltAngle;  // Up/down

    TargetArmRotation = FRotator(-TiltZ, 0.f, -TiltY);  // Pitch, Yaw, Roll
    CurrentArmRotation = FMath::RInterpTo(CurrentArmRotation, TargetArmRotation, DeltaTime, CameraTiltSpeed);

    SpringArm->SetRelativeRotation(CurrentArmRotation);

}

// Called to bind functionality to input
void APlayablePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    //basic 4 dirction movement
    PlayerInputComponent->BindAxis("MoveForward", this, &APlayablePlayer::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &APlayablePlayer::MoveRight);



}


void APlayablePlayer::MoveForward(float Value)
{
    LastInputZ = Value;

    if (Value != 0.0f)
    {
        FVector CurrentLocation = MeshRoot->GetRelativeLocation();
        float NewZ = FMath::Clamp(CurrentLocation.Z + (Value * 10.0f), -100.0f, 300.0f);
        MeshRoot->SetRelativeLocation(FVector(CurrentLocation.X, CurrentLocation.Y, NewZ));
    }
}

void APlayablePlayer::MoveRight(float Value)
     
{
    LastInputY = Value;

    if (Controller && Value != 0.0f)
    {
        FVector CurrentLocation = MeshRoot->GetRelativeLocation();
        float NewY = FMath::Clamp(CurrentLocation.Y + (Value * 10.0f), -200.0f, 200.0f);
        MeshRoot->SetRelativeLocation(FVector(CurrentLocation.X, NewY, CurrentLocation.Z));
    }
}


