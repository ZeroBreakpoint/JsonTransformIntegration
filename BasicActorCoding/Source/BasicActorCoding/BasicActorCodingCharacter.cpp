// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicActorCodingCharacter.h"
#include "BACGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Misc/Paths.h"

// Sets default values
ABasicActorCodingCharacter::ABasicActorCodingCharacter()
	: GameMode(nullptr)  // Initialises GameMode to nullptr
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialise the player's points to 0
	PlayerPoints = 0;

}

// Called when the game starts or when spawned
void ABasicActorCodingCharacter::BeginPlay()
{
	Super::BeginPlay();

	GameMode = GetWorld()->GetAuthGameMode<ABACGameMode>();
	if (IsValid(GameMode))
	{
		bool isDefaultLocation = GameMode->GetLastSavedPlayerLocation() == FVector::ZeroVector;
		if (!isDefaultLocation)
		{
			// Move character to last saved location
			MoveToLastSavedLocation();
		}
	}
	
}

void ABasicActorCodingCharacter::MoveToLastSavedLocation()
{
	if (IsValid(GameMode))
	{
		FVector LastSavedLocation = GameMode->GetLastSavedPlayerLocation();
		FRotator LastSavedRotation = GameMode->GetLastSavedPlayerRotation();

		if (LastSavedLocation != FVector::ZeroVector)
		{
			TeleportTo(LastSavedLocation, LastSavedRotation);
			UE_LOG(LogTemp, Warning, TEXT("Player moved to last saved location: %s"), *LastSavedLocation.ToString());
		}
	}
}

// Called every frame
void ABasicActorCodingCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABasicActorCodingCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// Method to add points to the player
void ABasicActorCodingCharacter::AddPoints(int32 Points)
{
	// Update the players score
	PlayerPoints += Points;

	// Log the current score
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Current Score: %d"), PlayerPoints));
	}


	if (GameMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("Calling ReportScore with PlayerPoints: %d"), PlayerPoints);
		GameMode->ReportScore(PlayerPoints);
		GameMode->SavePlayerPoints(PlayerPoints);
	}
}

// Method to save JSON file to path
FString ABasicActorCodingCharacter::CreateSaveFilePath(const FString& saveName)
{
	return FPaths::ProjectSavedDir() + "SaveJson/" + saveName + TEXT(".json.sav");
}

