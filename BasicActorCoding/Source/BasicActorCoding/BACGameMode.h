// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Misc/Paths.h"  // Includes the Paths header for file path operations
#include "BACSaveGame.h"  // Includes the header file for UBACSaveGame
#include "BACGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BASICACTORCODING_API ABACGameMode : public AGameModeBase
{
	GENERATED_BODY()

	// Used to store the running high score (either as loaded or attained during gameplay)
	int32 Highscore = 0;

	// Floating variable to keep track of time elapsed
	float TimeElapsed = 0.0f;

	// Stores the player location for saving
	FVector LastPlayerSavedLocation = FVector::ZeroVector;

	// Stores the player rotation for saving
	FRotator LastPlayerSavedRotation = FRotator::ZeroRotator;

	int32 PlayerPoints = 0;

public:
	// Set default classes
	ABACGameMode();

	// At the start of the level, load our data
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	// At the end of the level, save our data
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// This public function should be called whenever the player score changes.
	void ReportScore(int32 newScore);

	// Returns a copy of the player's last saved location
	FVector GetLastSavedPlayerLocation();

	// Returns a copy of the player's last saved rotation
	FRotator GetLastSavedPlayerRotation();

	// Functions for saving and getting player points for documented testing
	void SavePlayerPoints(int32 Points);

	int32 GetPlayerPoints() const;

	// Added for documented testing
	void TestSaveAndLoad();
	void RunTests();

	// Save the game data to a JSON file
	bool SaveGameToJSONFile(UBACSaveGame* saveGameObject, const FString& saveName);

private:

	// Added for documented testing
	void SaveGameTest();
	void LoadGameTest();

	// Function to create the save file path
	FString CreateSaveFilePath(const FString& saveName);

	//Function to load the saved JSON file
	USaveGame* LoadGameFromJSONFile(const FString& saveName);


	UBACSaveGame* LoadGame(const FString& saveName);

	bool SaveGame(const FString& saveName);

protected:
	UPROPERTY(EditDefaultsOnly)
	bool UseJSONSaveGame = true;

};
