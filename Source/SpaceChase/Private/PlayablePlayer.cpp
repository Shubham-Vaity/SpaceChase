// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayablePlayer.h"



#include "GameFramework/CharacterMovementComponent.h"  // This is the correct include for UCharacterMovementComponent
#include "GameFramework/SpringArmComponent.h"
#include "UObject/ConstructorHelpers.h"

#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"


// Sets default values
APlayablePlayer::APlayablePlayer()
{
    Tags.Add(FName("Player"));
//meshroot
    MeshRoot = CreateDefaultSubobject<USceneComponent>(TEXT("MeshRoot"));
    //gun
    FirePoint = CreateDefaultSubobject<UArrowComponent>(TEXT("GUN"));
   EnemType3 = CreateDefaultSubobject<UArrowComponent>(TEXT("EnemType3"));
    
    GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
   
    PrimaryActorTick.bCanEverTick = true;
     //SpringArm
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->TargetArmLength = 300.f;
    SpringArm->bUsePawnControlRotation = false;
    //Camera
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->bUsePawnControlRotation = false;


    GetMesh()->SetupAttachment(MeshRoot);
    EnemType3->SetupAttachment(SpringArm);
    MeshRoot->SetupAttachment(RootComponent);
    SpringArm->SetupAttachment(RootComponent);
    FirePoint->SetupAttachment(MeshRoot);
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);


    
  
    UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();  // Using GetCharacterMovement() to access the component

    if (CharacterMovementComponent)
    {
        CharacterMovementComponent->SetMovementMode(MOVE_Flying);
        CharacterMovementComponent->GravityScale = 0.0f;
        CharacterMovementComponent->MaxFlySpeed = 4000.0f; // Default is 600

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


    InitialCapsuleLocation = GetActorLocation();
    InitialMeshOffset = MeshRoot->GetRelativeLocation();

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

    //gun
    PlayerInputComponent->BindAction("Gun", IE_Pressed, this, &APlayablePlayer::FireAtMouse);

}


void APlayablePlayer::MoveForward(float Value)
{
    LastInputZ = Value;

    if (Value == 0.0f) return;

    // --- Move MESH (faster, more range)
    FVector MeshOffset = MeshRoot->GetRelativeLocation();
    float NewMeshZ = FMath::Clamp(MeshOffset.Z + (Value * MeshMoveSpeed), -MaxMeshOffsetZ, MaxMeshOffsetZ);
    MeshRoot->SetRelativeLocation(FVector(MeshOffset.X, MeshOffset.Y, NewMeshZ));

    // --- Move CAPSULE (slower, less range)
    FVector CapsuleLocation = GetActorLocation();
    float DeltaZ = CapsuleLocation.Z - InitialCapsuleLocation.Z;
    if ((DeltaZ < MaxCapsuleOffsetZ && Value > 0.f) || (DeltaZ > -MaxCapsuleOffsetZ && Value < 0.f))

    {
        FVector NewLocation = CapsuleLocation + FVector(0.f, 0.f, Value * CapsuleMoveSpeed);
        SetActorLocation(NewLocation);
    }
}

void APlayablePlayer::MoveRight(float Value)
{
    LastInputY = Value;

    if (Value == 0.0f) return;

    // --- Move MESH (fast, wide)
    FVector MeshOffset = MeshRoot->GetRelativeLocation();
    float NewMeshY = FMath::Clamp(MeshOffset.Y + (Value * MeshMoveSpeed), -MaxMeshOffsetY, MaxMeshOffsetY);
    MeshRoot->SetRelativeLocation(FVector(MeshOffset.X, NewMeshY, MeshOffset.Z));

    // --- Move CAPSULE (slower, narrow, but direction-aware)
    FVector CapsuleLocation = GetActorLocation();
    float DeltaY = CapsuleLocation.Y - InitialCapsuleLocation.Y;

    if ((DeltaY < MaxCapsuleOffsetY && Value > 0.f) || (DeltaY > -MaxCapsuleOffsetY && Value < 0.f))
    {
        FVector NewLocation = CapsuleLocation + FVector(0.f, Value * CapsuleMoveSpeed, 0.f);
        SetActorLocation(NewLocation);
    }
}


void APlayablePlayer::FireAtMouse()
{
    APlayerController* PC = Cast<APlayerController>(GetController());
    if (!PC || !Bullet1) return;

    // Get mouse position in screen space
    float MouseX, MouseY;
    if (PC->GetMousePosition(MouseX, MouseY))
    {
        // De-project mouse position to world
        FVector WorldLocation;
        FVector WorldDirection;

        if (PC->DeprojectScreenPositionToWorld(MouseX, MouseY, WorldLocation, WorldDirection))
        {
            FVector Start = FirePoint->GetComponentLocation(); // Start from arrow
            FVector End = Start + (WorldDirection * 20000.f);  // Long line in that direction

            // Do a line trace (optional)
            FHitResult HitResult;
            FCollisionQueryParams Params;
            Params.AddIgnoredActor(this); // Don't hit self

            bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);

            // Draw debug line
          //  DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.f, 0, 5.f);

            FVector TargetPoint = bHit ? HitResult.Location : End;

            // Calculate direction
            FVector Direction = (TargetPoint - Start).GetSafeNormal();

            // Spawn the bullet
            FActorSpawnParameters SpawnParams;
            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

            ABullet* Bullet = GetWorld()->SpawnActor<ABullet>(Bullet1, Start, Direction.Rotation(), SpawnParams);

            if (Bullet)
            {
                Bullet->SetActorRotation(Direction.Rotation());
                // You can also pass a velocity or target point to your bullet if needed
            }


        }
    }
}