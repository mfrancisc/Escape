// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "OpenDoor.h"

#define OUT
// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
    Owner = GetOwner();
}

void UOpenDoor::OpenTheDoor()
{
    Owner->SetActorRotation(FRotator(0.f, OpenAngle, 0.f));
}

void UOpenDoor::CloseTheDoor()
{
    Owner->SetActorRotation(FRotator(0.f, CloseAngle, 0.f));
}

bool UOpenDoor::TimeToCloseDoor()
{
    return LastDoorOpenTime + DoorCloseDelay <= GetWorld()->GetTimeSeconds();
}

// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// Poll the trigger volume
    if(GetTotalMassOfActorsOnPlate() > 30.f){// TOTO make into a parameter
        OpenTheDoor();
        LastDoorOpenTime = GetWorld()->GetTimeSeconds();
    }
    
    
    if(TimeToCloseDoor()){
        CloseTheDoor();
    }

}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
    float TotalMass = 0.f;
    TArray<AActor*> OverlappingActors;
    PressurePlate->GetOverlappingActors(OUT OverlappingActors);
    
    // Add total mass of objects
    for(const auto& Actor : OverlappingActors)
    {
        TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
    }
    
    return TotalMass;
}

