// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NPlayerController.generated.h"


class UInputMappingContext;
class UInputAction;

/**
 * 
 */
UCLASS()
class NBC_HW07_API ANPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ANPlayerController();

	UPROPERTY(EditAnywhere,Category="Input")
	UInputMappingContext* InputMappingContext;
	
	UPROPERTY(EditAnywhere,Category="Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* TiltAction;

protected:
	void BeginPlay() override;
};
