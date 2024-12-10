// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BACGameSaveTransformComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BASICACTORCODING_API UBACGameSaveTransformComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBACGameSaveTransformComponent();

	// Writes a transform to be saved
	bool SaveTransform(FTransform& saveTransform);

	// Applies the transform given
	void LoadTransform(const FTransform& loadTransform);

		
};
