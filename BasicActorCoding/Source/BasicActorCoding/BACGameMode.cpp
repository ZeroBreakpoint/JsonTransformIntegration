// Fill out your copyright notice in the Description page of Project Settings.


#include "BACGameMode.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"    // for save/load functions
#include "BACSaveGame.h"           // for our save game type
#include "EngineUtils.h"
#include "BasicActorCodingCharacter.h"
#include "BACGameSaveTransformComponent.h"

ABACGameMode::ABACGameMode()
{
    if (GEngine)
    {
        // Bind the RunTests function to the console command "RunTests" in Unreal Engine
        IConsoleManager::Get().RegisterConsoleCommand(
            TEXT("RunTests"),
            TEXT("Run the game mode tests."),
            FConsoleCommandDelegate::CreateUObject(this, &ABACGameMode::RunTests),
            ECVF_Cheat
        );

        UE_LOG(LogTemp, Warning, TEXT("RunTests console command registered"));
    }
}

void ABACGameMode::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Warning, TEXT("BeginPlay called in ABACGameMode"));

    UBACSaveGame* mySave = LoadGame(TEXT("SINGLEPLAYER"));

    if (IsValid(mySave))
    {
        FString currentLevelName = GetWorld()->GetMapName();
        currentLevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

        if (mySave->LevelName == currentLevelName)
        {
            Highscore = mySave->Highscore;
            LastPlayerSavedLocation = mySave->LastPlayerLocation;
            LastPlayerSavedRotation = mySave->LastPlayerRotation;
            PlayerPoints = mySave->PlayerPoints;

            for (TObjectIterator<UBACGameSaveTransformComponent> savedTransforms; savedTransforms; ++savedTransforms)
            {
                if (IsValid(*savedTransforms) && savedTransforms->GetWorld() == GetWorld())
                {
                    AActor* saveOwner = savedTransforms->GetOwner();
                    if (IsValid(saveOwner))
                    {
                        UE_LOG(LogTemp, Warning, TEXT("Checking actor: %s"), *saveOwner->GetActorNameOrLabel());
                        EWorldType::Type componentWorldType = savedTransforms->GetWorld()->WorldType;
                        if (componentWorldType == EWorldType::PIE || componentWorldType == EWorldType::Game)
                        {
                            if (FTransform* savedTransform = mySave->SaveTransformsByName.Find(saveOwner->GetActorNameOrLabel()))
                            {
                                savedTransforms->LoadTransform(*savedTransform);
                                UE_LOG(LogTemp, Warning, TEXT("Loaded transform for %s: %s"), *saveOwner->GetActorNameOrLabel(), *savedTransform->ToString());
                            }
                            else
                            {
                                UE_LOG(LogTemp, Warning, TEXT("No saved transform found for %s"), *saveOwner->GetActorNameOrLabel());
                            }
                        }
                    }
                }
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Saved game is for a different level: %s"), *mySave->LevelName);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No save game found, starting with default values."));
    }
}

void ABACGameMode::Tick(float DeltaSeconds)
{
}

void ABACGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    // Call EndPlay for Super class
    Super::EndPlay(EndPlayReason);

    // Log check
    UE_LOG(LogTemp, Warning, TEXT("EndPlay called in ABACGameMode"));

    // Save the game to slot "SINGLEPLAYER"
    if (SaveGame(TEXT("SINGLEPLAYER")))
    {
        UE_LOG(LogTemp, Warning, TEXT("Game saved successfully."));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to save game."));
    }

}

void ABACGameMode::ReportScore(int32 newScore)
{
    UE_LOG(LogTemp, Warning, TEXT("ReportScore called with newScore: %d"), newScore);

    // If newScore is greater, update, otherwise, no change
    if (newScore > Highscore)
    {
        Highscore = newScore;
        UE_LOG(LogTemp, Warning, TEXT("New Highscore: %d"), Highscore);  // Print high score to console

        // Save the game immediately to ensure the high score is saved
        USaveGame* newSave = UGameplayStatics::CreateSaveGameObject(UBACSaveGame::StaticClass());
        UBACSaveGame* mySave = Cast<UBACSaveGame>(newSave);

        if (IsValid(mySave))
        {
            mySave->Highscore = Highscore;
            mySave->LastPlayerLocation = LastPlayerSavedLocation;
            mySave->LastPlayerRotation = LastPlayerSavedRotation;
            mySave->PlayerPoints = PlayerPoints;

            for (TActorIterator<ABasicActorCodingCharacter> player(GetWorld()); player; ++player)
            {
                if (IsValid(*player))
                {
                    mySave->LastPlayerLocation = player->GetActorLocation();
                    mySave->LastPlayerRotation = player->GetActorRotation();
                    break;
                }
            }

            for (TObjectIterator<UBACGameSaveTransformComponent> savedTransforms; savedTransforms; ++savedTransforms)
            {
                if (IsValid(*savedTransforms) && savedTransforms->GetWorld() == GetWorld())
                {
                    AActor* saveOwner = savedTransforms->GetOwner();
                    if (IsValid(saveOwner))
                    {
                        EWorldType::Type ownerWorldType = saveOwner->GetWorld()->WorldType;
                        if (ownerWorldType == EWorldType::PIE || ownerWorldType == EWorldType::Game)
                        {
                            FTransform newSaveTransform;
                            savedTransforms->SaveTransform(newSaveTransform);
                            mySave->SaveTransformsByName.FindOrAdd(saveOwner->GetActorNameOrLabel(), newSaveTransform);
                        }
                    }
                }
            }

            UGameplayStatics::SaveGameToSlot(mySave, TEXT("SINGLEPLAYER"), 0);
        }
    }
}

FVector ABACGameMode::GetLastSavedPlayerLocation()
{
    return LastPlayerSavedLocation;
}

FRotator ABACGameMode::GetLastSavedPlayerRotation()
{
    return LastPlayerSavedRotation;
}

void ABACGameMode::SavePlayerPoints(int32 Points)
{
    PlayerPoints = Points;
}

int32 ABACGameMode::GetPlayerPoints() const
{
    return PlayerPoints;
}

void ABACGameMode::SaveGameTest()
{
    UE_LOG(LogTemp, Warning, TEXT("Running SaveGameTest"));

    // Set some test values
    ReportScore(150);
    SavePlayerPoints(75);

    // Log values before saving
    UE_LOG(LogTemp, Warning, TEXT("Before SaveGameTest - Highscore: %d, PlayerPoints: %d"), Highscore, PlayerPoints);

    // Perform save operation
    USaveGame* newSave = UGameplayStatics::CreateSaveGameObject(UBACSaveGame::StaticClass());
    UBACSaveGame* mySave = Cast<UBACSaveGame>(newSave);

    if (!IsValid(mySave))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create save game object"));
        return;
    }

    mySave->Highscore = Highscore;
    mySave->LastPlayerLocation = LastPlayerSavedLocation;
    mySave->LastPlayerRotation = LastPlayerSavedRotation;
    mySave->PlayerPoints = PlayerPoints;
    mySave->LevelName = GetWorld()->GetMapName();
    mySave->LevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

    for (TActorIterator<ABasicActorCodingCharacter> player(GetWorld()); player; ++player)
    {
        if (IsValid(*player))
        {
            mySave->LastPlayerLocation = player->GetActorLocation();
            mySave->LastPlayerRotation = player->GetActorRotation();
            break;
        }
    }

    for (TObjectIterator<UBACGameSaveTransformComponent> savedTransforms; savedTransforms; ++savedTransforms)
    {
        if (IsValid(*savedTransforms) && savedTransforms->GetWorld() == GetWorld())
        {
            AActor* saveOwner = savedTransforms->GetOwner();
            if (IsValid(saveOwner))
            {
                EWorldType::Type ownerWorldType = saveOwner->GetWorld()->WorldType;
                if (ownerWorldType == EWorldType::PIE || ownerWorldType == EWorldType::Game)
                {
                    FTransform newSaveTransform;
                    savedTransforms->SaveTransform(newSaveTransform);
                    mySave->SaveTransformsByName.FindOrAdd(saveOwner->GetActorNameOrLabel(), newSaveTransform);
                }
            }
        }
    }

    if (UGameplayStatics::SaveGameToSlot(mySave, TEXT("SINGLEPLAYER"), 0))
    {
        UE_LOG(LogTemp, Warning, TEXT("Game saved successfully."));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to save game."));
    }
}

void ABACGameMode::LoadGameTest()
{
    UE_LOG(LogTemp, Warning, TEXT("Running LoadGameTest"));

    // Log values before loading
    UE_LOG(LogTemp, Warning, TEXT("Before LoadGameTest - Highscore: %d, PlayerPoints: %d"), Highscore, PlayerPoints);

    // Perform load operation
    USaveGame* loadSave = UGameplayStatics::LoadGameFromSlot(TEXT("SINGLEPLAYER"), 0);
    if (!loadSave)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load save game from slot"));
        return;
    }

    UBACSaveGame* mySave = Cast<UBACSaveGame>(loadSave);
    if (!IsValid(mySave))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to cast save game to UBACSaveGame"));
        return;
    }

    FString currentLevelName = GetWorld()->GetMapName();
    currentLevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

    if (mySave->LevelName != currentLevelName)
    {
        UE_LOG(LogTemp, Warning, TEXT("Saved game is for a different level: %s"), *mySave->LevelName);
        return;
    }

    Highscore = mySave->Highscore;
    LastPlayerSavedLocation = mySave->LastPlayerLocation;
    LastPlayerSavedRotation = mySave->LastPlayerRotation;
    PlayerPoints = mySave->PlayerPoints;

    UE_LOG(LogTemp, Warning, TEXT("Loaded save game data: Highscore=%d, PlayerPoints=%d"), Highscore, PlayerPoints);

    for (TObjectIterator<UBACGameSaveTransformComponent> savedTransforms; savedTransforms; ++savedTransforms)
    {
        if (IsValid(*savedTransforms) && savedTransforms->GetWorld() == GetWorld())
        {
            AActor* saveOwner = savedTransforms->GetOwner();
            if (IsValid(saveOwner))
            {
                EWorldType::Type componentWorldType = savedTransforms->GetWorld()->WorldType;
                if (componentWorldType == EWorldType::PIE || componentWorldType == EWorldType::Game)
                {
                    if (FTransform* savedTransform = mySave->SaveTransformsByName.Find(saveOwner->GetActorNameOrLabel()))
                    {
                        savedTransforms->LoadTransform(*savedTransform);
                    }
                    else
                    {
                        UE_LOG(LogTemp, Warning, TEXT("No saved transform found for %s"), *saveOwner->GetActorNameOrLabel());
                    }
                }
            }
        }
    }

    // Log values after loading
    UE_LOG(LogTemp, Warning, TEXT("After LoadGameTest - Highscore: %d, PlayerPoints: %d"), Highscore, PlayerPoints);

    // Verify values
    bool HighscoreCorrect = (Highscore == 150);
    bool PlayerPointsCorrect = (GetPlayerPoints() == 75);

    UE_LOG(LogTemp, Warning, TEXT("LoadGameTest - HighscoreCorrect: %s, PlayerPointsCorrect: %s"),
        HighscoreCorrect ? TEXT("true") : TEXT("false"),
        PlayerPointsCorrect ? TEXT("true") : TEXT("false"));

    ensure(HighscoreCorrect);
    ensure(PlayerPointsCorrect);
}

void ABACGameMode::TestSaveAndLoad()
{
    SaveGameTest();
    LoadGameTest();
}

void ABACGameMode::RunTests()
{
    TestSaveAndLoad();
}

bool ABACGameMode::SaveGameToJSONFile(UBACSaveGame* saveGameObject, const FString& saveName)
{
    // Create JSON object
    TSharedPtr<FJsonObject> saveJson = MakeShareable(new FJsonObject());
    // Write JSON properties
    saveJson->SetNumberField(TEXT("Highscore"), saveGameObject->Highscore);
    saveJson->SetNumberField(TEXT("PlayerPoints"), saveGameObject->PlayerPoints);
    saveJson->SetStringField(TEXT("LastPlayerLocation"), saveGameObject->LastPlayerLocation.ToString());
    saveJson->SetStringField(TEXT("LastPlayerRotation"), saveGameObject->LastPlayerRotation.ToString());
    saveJson->SetStringField(TEXT("LevelName"), saveGameObject->LevelName);

    TArray<TSharedPtr<FJsonValue>> jsonTransformValues;
    for (auto& transformPair : saveGameObject->SaveTransformsByName)
    {
        TSharedPtr<FJsonObject> saveTransform = MakeShareable(new FJsonObject());

        saveTransform->SetStringField(TEXT("Name"), transformPair.Key);
        saveTransform->SetStringField(TEXT("Transform"), transformPair.Value.ToString());

        jsonTransformValues.Add(MakeShareable(new FJsonValueObject(saveTransform)));
    }
    saveJson->SetArrayField(TEXT("SaveTransformsByName"), jsonTransformValues);

    // Serialize JSON to string for storage
    FString contents;
    TSharedRef<TJsonWriter<>> saveWriter = TJsonWriterFactory<>::Create(&contents);
    FJsonSerializer::Serialize(saveJson.ToSharedRef(), saveWriter);

    // Write JSON string to file
    const FString saveGamePath = CreateSaveFilePath(saveName);
    bool saveSuccess = FFileHelper::SaveStringToFile(contents, *saveGamePath);

    // Return whether write was successful
    return saveSuccess;
}

// Function to create the save file path
FString ABACGameMode::CreateSaveFilePath(const FString& saveName)
{
    return FPaths::ProjectSavedDir() + "SaveJson/" + saveName + TEXT(".json.sav");
}

USaveGame* ABACGameMode::LoadGameFromJSONFile(const FString& saveName)
{
    const FString saveGamePath = CreateSaveFilePath(saveName);
    UE_LOG(LogTemp, Warning, TEXT("Loading save game from path: %s"), *saveGamePath);

    FString contents;
    bool loadSuccess = FFileHelper::LoadFileToString(contents, *saveGamePath);
    if (!loadSuccess) {
        UE_LOG(LogTemp, Error, TEXT("Failed to load file from path: %s"), *saveGamePath);
        return nullptr;
    }

    UBACSaveGame* saveGameInstance = Cast<UBACSaveGame>(UGameplayStatics::CreateSaveGameObject(UBACSaveGame::StaticClass()));
    TSharedPtr<FJsonObject> saveJson = MakeShareable(new FJsonObject());

    TSharedRef<TJsonReader<>> saveReader = TJsonReaderFactory<>::Create(contents);
    if (FJsonSerializer::Deserialize(saveReader, saveJson) && saveJson.IsValid())
    {
        saveJson->TryGetNumberField(TEXT("Highscore"), saveGameInstance->Highscore);
        saveJson->TryGetNumberField(TEXT("PlayerPoints"), saveGameInstance->PlayerPoints);
        FString lastPlayerLocation;
        saveJson->TryGetStringField(TEXT("LastPlayerLocation"), lastPlayerLocation);
        saveGameInstance->LastPlayerLocation.InitFromString(lastPlayerLocation);
        FString lastPlayerRotation;
        saveJson->TryGetStringField(TEXT("LastPlayerRotation"), lastPlayerRotation);
        saveGameInstance->LastPlayerRotation.InitFromString(lastPlayerRotation);

        FString levelName;
        saveJson->TryGetStringField(TEXT("LevelName"), levelName);
        saveGameInstance->LevelName = levelName;




        const TArray<TSharedPtr<FJsonValue>>* saveTransforms;
        bool hasSaveTransforms = saveJson->TryGetArrayField(TEXT("SaveTransformsByName"), saveTransforms);
        if (hasSaveTransforms)
        {
            for (auto& transformPair : *saveTransforms)
            {
                TSharedPtr<FJsonObject> saveTransform = transformPair->AsObject();
                FString transformName;
                saveTransform->TryGetStringField(TEXT("Name"), transformName);
                FString transformValue;
                saveTransform->TryGetStringField(TEXT("Transform"), transformValue);
                FTransform transformParsed;
                transformParsed.InitFromString(transformValue);

                saveGameInstance->SaveTransformsByName.Add(transformName, transformParsed);
                UE_LOG(LogTemp, Warning, TEXT("Loaded transform for %s: %s"), *transformName, *transformValue);
            }
        }
    }

    return saveGameInstance;
}


UBACSaveGame* ABACGameMode::LoadGame(const FString& saveName)
{
    // Intermediate save data object as loaded from file
    USaveGame* loadSave = nullptr;

    if (UseJSONSaveGame)
    {
        loadSave = LoadGameFromJSONFile(saveName);
    }
    else
    {
        loadSave = UGameplayStatics::LoadGameFromSlot(saveName, 0);
    }

    UBACSaveGame* mySave = Cast<UBACSaveGame>(loadSave);

    // If the save is valid and can be used...
    if (IsValid(mySave))
    {
        FString currentLevelName = GetWorld()->GetMapName();
        currentLevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

        if (mySave->LevelName == currentLevelName)
        {
            // Safe to read from properties here

            // Copy the saved Highscore into our Game Mode
            Highscore = mySave->Highscore;
            LastPlayerSavedLocation = mySave->LastPlayerLocation;
            LastPlayerSavedRotation = mySave->LastPlayerRotation;
            PlayerPoints = mySave->PlayerPoints;
            return mySave;
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Saved game is for a different level: %s"), *mySave->LevelName);
        }
    }

    return nullptr;
}

bool ABACGameMode::SaveGame(const FString& saveName)
{
    // Create a copy of the save data
    USaveGame* newSave = UGameplayStatics::CreateSaveGameObject(UBACSaveGame::StaticClass());
    // Cast it to our save data type
    UBACSaveGame* mySave = Cast<UBACSaveGame>(newSave);

    // If the save is valid and can be used...
    if (IsValid(mySave))
    {
        // ... write the properties we need to save.

        // Copy the best highscore into the save data object
        mySave->Highscore = Highscore;
        // Save the last player location
        mySave->PlayerPoints = PlayerPoints;
        mySave->LevelName = GetWorld()->GetMapName();
        mySave->LevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
        for (TActorIterator<ABasicActorCodingCharacter> player(GetWorld()); player; ++player)
        {
            // Skip invalid players
            if (IsValid(*player))
            {
                mySave->LastPlayerLocation = player->GetActorLocation();
                mySave->LastPlayerRotation = player->GetActorRotation();
                break;
            }
        }

        // Save persistant transforms
        for (TObjectIterator<UBACGameSaveTransformComponent> savedTransforms; savedTransforms; ++savedTransforms)
        {
            // Skip invalid objects
            if (IsValid(*savedTransforms) && savedTransforms->GetWorld() == GetWorld())
            {

                AActor* saveOwner = savedTransforms->GetOwner();
                if (IsValid(saveOwner))
                {
                    // Don't load if object is not from a game world
                    EWorldType::Type ownerWorldType = saveOwner->GetWorld()->WorldType;
                    if (ownerWorldType == EWorldType::PIE || ownerWorldType == EWorldType::Game)
                    {

                        // Associate name with transform information
                        
                        FTransform newSaveTransform;
                        savedTransforms->SaveTransform(newSaveTransform);
                        mySave->SaveTransformsByName.FindOrAdd(saveOwner->GetActorNameOrLabel(), newSaveTransform);
                    }
                }
            }
        }

        if (UseJSONSaveGame)
        {
            return SaveGameToJSONFile(mySave, saveName);
        }
        else
        {
            return UGameplayStatics::SaveGameToSlot(mySave, saveName, 0);
        }
    }

    return false;
}





