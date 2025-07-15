// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "NPlanePawn.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UStaticMeshComponent;
class UBoxComponent;
struct FInputActionValue;

UCLASS()
class NBC_HW07_API ANPlanePawn : public APawn
{
	GENERATED_BODY()

public:
	ANPlanePawn();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void Move(const FInputActionValue& Value);

	UFUNCTION()
	void Look(const FInputActionValue& Value);

	UFUNCTION()
	void Tilt(const FInputActionValue& Value);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Capsule")
	UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Camera")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* StaticMeshComponent;
private:
	float MoveSpeed;
	float MinMoveSpeed;
	float MaxMoveSpeed;
	float RotSpeed;

	float GravityZ;
	float GravityPower;

	bool bIsCollsionWithGround();
	void ControlSpeed();
};
