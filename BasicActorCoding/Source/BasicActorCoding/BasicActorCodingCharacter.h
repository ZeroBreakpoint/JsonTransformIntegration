// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Misc/Paths.h" // Includes the Paths header for file path operations
#include "BasicActorCodingCharacter.generated.h"

UCLASS()
class BASICACTORCODING_API ABasicActorCodingCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABasicActorCodingCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Method to add points to the player
	void AddPoints(int32 Points);

	// Function to save JSON file to path
	FString CreateSaveFilePath(const FString& saveName);

private:
	// The player's current points
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Points", meta = (AllowPrivateAccess = "true"))
	int32 PlayerPoints;

	// Reference to the game mode
	class ABACGameMode* GameMode;

	// Ensures the player is moved to the last saved location
	void MoveToLastSavedLocation();

};
