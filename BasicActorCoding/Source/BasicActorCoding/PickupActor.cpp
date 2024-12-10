// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupActor.h"
#include "Components/StaticMeshComponent.h"
#include "BasicActorCodingCharacter.h"

// Sets default values
APickupActor::APickupActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a default instance of our pickup mesh
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));

	// Configure the PickupMesh to be able to overlap other Actors
	PickupMesh->SetCollisionProfileName(FName(TEXT("OverlapAll")));

	// Set the PickupMesh as the root component
	RootComponent = PickupMesh;

}

// Called when the game starts or when spawned
void APickupActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickupActor::Tick(float DeltaTime)
{
	// Calling the parents' class version of function
	Super::Tick(DeltaTime);  

	// This was "Get Game Time in Seconds" in Blueprints
	float GameSeconds = GetWorld()->GetTimeSeconds();
	
	// NOTE: The C++ version works with radians, not degrees, so we need to convert (RotationSpeed in radians)
	float AngleInRadians = FMath::DegreesToRadians(GameSeconds) * RotationSpeed;
	
	// This was "Rotator from Axis and Angle" in Blueprints
	FRotator NewRotation = FRotator(FQuat(FVector(0.0f, 0.0f, 1.0f), AngleInRadians));
	
	// This was "Set Actor Rotation" in Blueprints
	SetActorRotation(NewRotation);

}

void APickupActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	// Always call the Super version of the function first unless
	// you know what you're doing
	Super::NotifyActorBeginOverlap(OtherActor);

	// Cast to ABasicActorCodingCharacter
	ABasicActorCodingCharacter* player = Cast<ABasicActorCodingCharacter>(OtherActor);
		// Check if cast was successful and object is usable
		if (IsValid(player))
		 {

			// Award points to the player
			player->AddPoints(Points);
			
			// Destroy this Actor
			Destroy();
		}
}

