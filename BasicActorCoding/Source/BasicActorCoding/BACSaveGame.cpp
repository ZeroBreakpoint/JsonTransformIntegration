// Fill out your copyright notice in the Description page of Project Settings.


#include "BACSaveGame.h"

UBACSaveGame::UBACSaveGame()
{
	Highscore = 0;
	LastPlayerLocation = FVector::ZeroVector;
	LastPlayerRotation = FRotator::ZeroRotator;
	LevelName = TEXT("");  // Initialise the LevelName
	PlayerPoints = 0;
}

