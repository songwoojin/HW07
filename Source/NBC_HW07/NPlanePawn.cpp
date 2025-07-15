// Fill out your copyright notice in the Description page of Project Settings.


#include "NPlanePawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "EnhancedInputComponent.h"
#include "NPlayerController.h"

ANPlanePawn::ANPlanePawn()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetBoxExtent(FVector(150.f, 200.f, 50.f), true);
	SetRootComponent(BoxComponent);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 600.0f;
	SpringArmComponent->bUsePawnControlRotation = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	StaticMeshComponent->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	MaxMoveSpeed = 3000.0f;
	MinMoveSpeed = 1800.0f;
	MoveSpeed = MaxMoveSpeed;
	RotSpeed = 60.0f;
	GravityZ = -9.8f;
	GravityPower = 0;
}

void ANPlanePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void ANPlanePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ControlSpeed();
}

void ANPlanePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ANPlayerController* PlayerController = Cast<ANPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, this, &ANPlanePawn::Move);
			}
			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(PlayerController->LookAction, ETriggerEvent::Triggered, this, &ANPlanePawn::Look);
			}
			if (PlayerController->TiltAction)
			{
				EnhancedInput->BindAction(PlayerController->TiltAction, ETriggerEvent::Triggered, this, &ANPlanePawn::Tilt);
			}
		}
	}

}

void ANPlanePawn::Move(const FInputActionValue& Value)
{
	if (!Controller)	return;

	const FVector MoveInput = (Value.Get<FVector>()).GetSafeNormal();

	const float Dt = GetWorld()->GetDeltaSeconds();
	FVector Speed = MoveInput* MoveSpeed * Dt;

	if (!FMath::IsNearlyZero(MoveInput.X) || !FMath::IsNearlyZero(MoveInput.Y) || !FMath::IsNearlyZero(MoveInput.Z))
	{
		//Sweep 켜주기
		AddActorLocalOffset(Speed,true);
	}
}

void ANPlanePawn::Look(const FInputActionValue& Value)
{
	const FVector2D Input = Value.Get<FVector2D>();
	const float Dt = GetWorld()->GetDeltaSeconds();

	const float Yaw = Input.X * RotSpeed * Dt;
	const float Pitch = Input.Y * RotSpeed * Dt;

	//같이 적용시키면 쿼터니언 발생->Roll도 회전
	AddActorLocalRotation(FRotator(0.0f, Yaw, 0.0f));
	AddActorLocalRotation(FRotator(Pitch, 0.0f, 0.0f));
}

void ANPlanePawn::Tilt(const FInputActionValue& Value)
{
	const float Input = Value.Get<float>();
	const float DT = GetWorld()->GetDeltaSeconds();

	float CurrentRoll = Input * RotSpeed * DT;

	// ① Pawn에 Roll 회전 적용
	AddActorLocalRotation(FRotator(0.f, 0.f, CurrentRoll), false);
}

bool ANPlanePawn::bIsCollsionWithGround()
{
	FHitResult Hit;
	FVector Start = GetActorLocation();
	FVector End = Start + FVector(0.f, 0.f, -100.0f);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		Hit, Start, End, ECC_Visibility, Params
	);

	return bHit;
}

void ANPlanePawn::ControlSpeed()
{
	if (bIsCollsionWithGround())
	{
		GravityPower = 0.0f;

		MoveSpeed = MaxMoveSpeed;

		UE_LOG(LogTemp, Warning, TEXT("Collision Wtih Ground"));
	}
	else
	{
		GravityPower += GravityZ * GetWorld()->GetDeltaSeconds();
		//폰이 뒤집힌 상태일 경우도 있으므로 월드에서 변경
		AddActorWorldOffset(FVector(0.0f, 0.0f, GravityPower));
		MoveSpeed = MinMoveSpeed;
	}
}

