// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "BACSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class BASICACTORCODING_API UBACSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UBACSaveGame();

	// A variable for storing the player's highscore
	UPROPERTY()
	int32 Highscore = 0;

	// A variable for storing the player's last location
	UPROPERTY()
	FVector LastPlayerLocation = FVector::ZeroVector;

	// A variable for storing the player's last rotation
	UPROPERTY()
	FRotator LastPlayerRotation = FRotator::ZeroRotator;

	// Map of Transforms
	//		- the KEY (FString)is the NAME of the actor
	//		- the VALUE (FTransform) is the TRANSFORM (location, rotation and scale) of the actor at the time we saved
	UPROPERTY()
	TMap<FString, FTransform> SaveTransformsByName;

	// A variable for storing the player's points
	UPROPERTY()
	int32 PlayerPoints = 0;

	// A variable for storing the name of the level
	UPROPERTY()
	FString LevelName;

};