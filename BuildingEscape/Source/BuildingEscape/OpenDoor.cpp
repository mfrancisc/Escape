// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "OpenDoor.h"


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

    ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
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
    // if the actor that opens is in the volume
    if(PressurePlate->IsOverlappingActor(ActorThatOpens)){
        OpenTheDoor();
        LastDoorOpenTime = GetWorld()->GetTimeSeconds();
    }
    
    
    if(TimeToCloseDoor()){
        CloseTheDoor();
    }

}

