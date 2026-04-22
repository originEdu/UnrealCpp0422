// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABasicPlayer::ABasicPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//스프링 암 & 카메라 생성
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	//Mesh 위치조정
	GetMesh()->SetRelativeLocationAndRotation(
		FVector(0,0,-1 * GetCapsuleComponent()->GetScaledCapsuleHalfHeight()),
		FRotator(0,-90.0f,0)
	);
	
}

// Called when the game starts or when spawned
void ABasicPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABasicPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABasicPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* UIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (UIC)
	{
		UIC->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ABasicPlayer::Move);
		UIC->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ABasicPlayer::Look);
		UIC->BindAction(IA_Jump, ETriggerEvent::Started, this, &ABasicPlayer::Jump);
		UIC->BindAction(IA_Jump, ETriggerEvent::Completed, this, &ABasicPlayer::StopJumping);
		UIC->BindAction(IA_Jump, ETriggerEvent::Canceled, this, &ABasicPlayer::StopJumping);
	}
}

void ABasicPlayer::Move(const FInputActionValue& Value)
{
	FVector2D Direction = Value.Get<FVector2D>();

	//카메라방향 앞방향으로 움직이게 설정
	//이때 바닥면에 수평인 방향벡터를 구해야한다
	//폰 회전이나 스프링암 회전은 컨트롤러 회전에 따른 정의로 회전함
	FRotator CameraRotation = GetControlRotation();
	FRotator CameraRotationInFloor = FRotator(0, CameraRotation.Yaw,0);
	FVector CameraForwardInFloor = UKismetMathLibrary::GetForwardVector(CameraRotationInFloor);
	FVector CameraRightInFloor = UKismetMathLibrary::GetRightVector(CameraRotationInFloor);

	AddMovementInput(CameraForwardInFloor * Direction.X);
	AddMovementInput(CameraRightInFloor * Direction.Y);
}

void ABasicPlayer::Look(const FInputActionValue& Value)
{
	FVector2D Rotation = Value.Get<FVector2D>();

	AddControllerYawInput(Rotation.X);
	AddControllerPitchInput(-Rotation.Y);
}
