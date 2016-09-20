// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "Grabber.h"

#define OUT


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UGrabber::Grab() {
     UE_LOG(LogTemp, Warning, TEXT("Grab key pressed"))
}

void UGrabber::Release() {
    UE_LOG(LogTemp, Warning, TEXT("Grab key released"))
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Grapper reporting for duty"));
    
    /// Look for attached Physics Handle
    PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
    if(PhysicsHandle)
    {
        UE_LOG(LogTemp, Warning, TEXT("Component PhysicsHandleComponent exists"))
    } else
    {
        UE_LOG(LogTemp, Error, TEXT("Component PhysicsHandleComponent missing from: %s"), *(GetOwner()->GetName()))
    }
    
    /// Look for attached Input Component
    InputHandle = GetOwner()->FindComponentByClass<UInputComponent>();
    if(InputHandle){
        UE_LOG(LogTemp, Warning, TEXT("Component InputComponent exists"));
        /// Bind the input axis
        InputHandle->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
        InputHandle->BindAction("Grab", IE_Released, this, &UGrabber::Release);
    }else
    {
        UE_LOG(LogTemp, Error, TEXT("Component InputComponent missing from: %s"), *(GetOwner()->GetName()))
    }
}


// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
    
    // get player view point this tick
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
                OUT PlayerViewPointLocation,
                OUT PlayerViewPointRotation
                                                               );
   /* UE_LOG(LogTemp, Warning, TEXT("Player Location: %s , Player Rotation: %s "),
           *PlayerViewPointLocation.ToString(),
           *PlayerViewPointRotation.ToString()
           );*/
    
    FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
    
    DrawDebugLine(
        GetWorld(),
        PlayerViewPointLocation,
        LineTraceEnd,
        FColor(255,0,0),
        false,
        0.f,
        0.f,
        10.f
    );
    
    ///Setyp query parameters
    FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
    
    /// Line-trace (AKA ray-cast) out to reach distance
    FHitResult Hit;
    GetWorld()->LineTraceSingleByObjectType(
        OUT Hit,
        PlayerViewPointLocation,
        LineTraceEnd,
        FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
        TraceParameters
    );
    
    FString Hitted;
    if(Hit.GetActor()){
        Hitted = Hit.GetActor()->GetName();
        UE_LOG(LogTemp, Warning, TEXT("Object name: %s"), *Hitted);
    }
    
    
    
}

