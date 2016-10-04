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
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
    Owner = GetOwner();
    
    if(!PressurePlate){
        UE_LOG(LogTemp, Error, TEXT("Component PressurePlate missing from: %s"), *(GetOwner()->GetName()))
    }
}

// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// Poll the trigger volume
    if(GetTotalMassOfActorsOnPlate() > TriggerMass){
        OnOpen.Broadcast();
    }else{
        OnClose.Broadcast();
    }

}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
    float TotalMass = 0.f;
    TArray<AActor*> OverlappingActors;
    
    if(!PressurePlate) return TotalMass;
    
    PressurePlate->GetOverlappingActors(OUT OverlappingActors);
    
    // Add total mass of objects
    for(const auto& Actor : OverlappingActors)
    {
        TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
    }
    
    return TotalMass;
}

