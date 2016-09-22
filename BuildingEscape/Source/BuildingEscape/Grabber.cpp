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
}

void UGrabber::Grab()
{
     UE_LOG(LogTemp, Warning, TEXT("Grab key pressed"))
    
    /// Try and reach any actors with physics body collision channel set
    auto HitResult = GetfirstPhysicsBodyInReach();
    auto ComponentToGrab = HitResult.GetComponent();
    auto ActorHit = HitResult.GetActor();
    
    /// if we hit shomething then attach a physics handle
    if(ActorHit){
    /// Attach physics handle rotation
    PhysicsHandle->GrabComponent(
                            ComponentToGrab,
                            NAME_None,
                            ComponentToGrab->GetOwner()->GetActorLocation(),
                            true // allow rotation
        );
    }
}


void UGrabber::Release()
{
    UE_LOG(LogTemp, Warning, TEXT("Grab key released"))
    
    /// release physics handle
    PhysicsHandle->ReleaseComponent();
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
    FindPhysicsHandleComponent();
    SetUpInputComponent();
}

/// Look for attached Input Component
void UGrabber::SetUpInputComponent()
{
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

void UGrabber::FindPhysicsHandleComponent()
{
    /// Look for attached Physics Handle
    PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
    if(PhysicsHandle)
    {
        UE_LOG(LogTemp, Warning, TEXT("Component PhysicsHandleComponent exists"))
    } else
    {
        UE_LOG(LogTemp, Error, TEXT("Component PhysicsHandleComponent missing from: %s"), *(GetOwner()->GetName()))
    }
}

// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
    
    if(PhysicsHandle->GrabbedComponent)
    {
        // move the object that we're holding
        PhysicsHandle->SetTargetLocation(GetLineTraceEnd());
        
    }
}


const FHitResult UGrabber::GetfirstPhysicsBodyInReach()
{
        
        ///Setyp query parameters
        FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
        
        /// Line-trace (AKA ray-cast) out to reach distance
        FHitResult Hit;
        GetWorld()->LineTraceSingleByObjectType(
                                                OUT Hit,
                                                GetPlayerViewPointLocation(),
                                                GetLineTraceEnd(),
                                                FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
                                                TraceParameters
                                                );
        
        FString Hitted;
        if(Hit.GetActor()){
            Hitted = Hit.GetActor()->GetName();
            UE_LOG(LogTemp, Warning, TEXT("Object name: %s"), *Hitted);
        }
        
        return Hit;
    
}

/// Set palyer location and reacheable distance
void UGrabber::SetPlayerViewPoint()
{
    FRotator PlayerViewPointRotation;
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
                                                               OUT PlayerViewPointLocation,
                                                               OUT PlayerViewPointRotation
                                                               );
    
    
    LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}

FVector UGrabber::GetPlayerViewPointLocation() {
    SetPlayerViewPoint();
    
    return PlayerViewPointLocation;
}

FVector UGrabber::GetLineTraceEnd() {
    SetPlayerViewPoint();
    
    return LineTraceEnd;
}
