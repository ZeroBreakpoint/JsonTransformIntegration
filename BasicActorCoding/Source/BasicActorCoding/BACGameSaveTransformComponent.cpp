// Fill out your copyright notice in the Description page of Project Settings.


#include "BACGameSaveTransformComponent.h"
#include "Engine/Engine.h"

// Sets default values for this component's properties
UBACGameSaveTransformComponent::UBACGameSaveTransformComponent()
{
	// Setting tick to false as this is not needed
	PrimaryComponentTick.bCanEverTick = false;

    if (AActor* Owner = GetOwner())
    {
        UE_LOG(LogTemp, Warning, TEXT("UBACGameSaveTransformComponent initialised for %s"), *Owner->GetActorNameOrLabel());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("UBACGameSaveTransformComponent initialised for an actor with no owner"));
    }
}

bool UBACGameSaveTransformComponent::SaveTransform(FTransform& saveTransform)
{
    if (AActor* Owner = GetOwner())
    {
        saveTransform = Owner->GetActorTransform();
        UE_LOG(LogTemp, Warning, TEXT("Saved Transform for %s: %s"), *Owner->GetActorNameOrLabel(), *saveTransform.ToString());
        return true;
    }
    UE_LOG(LogTemp, Error, TEXT("Failed to save transform for %s"), *GetOwner()->GetActorNameOrLabel());
    return false;
}

void UBACGameSaveTransformComponent::LoadTransform(const FTransform& loadTransform)
{
    if (AActor* Owner = GetOwner())
    {
        Owner->SetActorTransform(loadTransform);
        UE_LOG(LogTemp, Warning, TEXT("Loaded Transform for %s: %s"), *Owner->GetActorNameOrLabel(), *loadTransform.ToString());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load transform for %s"), *GetOwner()->GetActorNameOrLabel());
    }
}


