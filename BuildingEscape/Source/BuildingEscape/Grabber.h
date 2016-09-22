// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

private:
    
    // How far a head of the player ca we reach in cm
    float Reach = 100.f;
    
    FVector PlayerViewPointLocation;
    
    FVector LineTraceEnd;
		
    UPhysicsHandleComponent* PhysicsHandle = nullptr;
    
    UInputComponent* InputHandle = nullptr;
    
    // Ray-cast and grab whats in reach
    void Grab();
	
    // Called when grab key is released
    void Release();
    
    // find attached physics component
    void FindPhysicsHandleComponent();
    
    void SetUpInputComponent();
    
    // Return hit for first physics body in reach
    const FHitResult GetfirstPhysicsBodyInReach();
    
    void SetPlayerViewPoint();
    
    // Return current player location
    FVector GetReachLineStart();
    
    // Return end of reach distance
    FVector GetReachLineEnd();
};

