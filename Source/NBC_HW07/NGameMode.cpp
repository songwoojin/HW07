// Fill out your copyright notice in the Description page of Project Settings.


#include "NGameMode.h"
#include "NPlanePawn.h"
#include "NPlayerController.h"

ANGameMode::ANGameMode()
{
	DefaultPawnClass = ANPlanePawn::StaticClass();
	PlayerControllerClass = ANPlayerController::StaticClass();
}
