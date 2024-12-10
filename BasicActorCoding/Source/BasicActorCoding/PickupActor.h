// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupActor.generated.h"

UCLASS()
class BASICACTORCODING_API APickupActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when this Actor overlaps with another Actor
	virtual void NotifyActorBeginOverlap(AActor * OtherActor) override;

public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RotationSpeed = 90.0f;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* PickupMesh;

	// Points awarded to the player when collecting this pickup
	// Exposed for editing in the Unreal Engine
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (AllowPrivateAccess = "true"))
	int32 Points = 5;

};
