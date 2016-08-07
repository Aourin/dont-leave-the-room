// Fill out your copyright notice in the Description page of Project Settings.

#include "DontLeave.h"
#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
	Reach = 100.f;
	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindInputHandler();
	FindPhysicsComponent();
}

// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if (PhysicsHandle->GrabbedComponent)
	{
		FVector HitLocation = GetReachPoint();
		PhysicsHandle->SetTargetLocation(HitLocation);
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab Item"));
	auto Hit = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = Hit.GetComponent();
	if (ComponentToGrab)
	{
		TargetActor = Hit.GetActor();
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None,
			TargetActor->GetActorLocation(),
			true
			);
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Release Item"));
	PhysicsHandle->ReleaseComponent();
	TargetActor = nullptr;
}

const FVector UGrabber::GetReachPoint()
{
	LocRot PlayerLocRot = GetPlayerLocRot();
	
	//	Get Trace by Reach Distance
	FVector LineTraceEnd = PlayerLocRot.Location + PlayerLocRot.Rotator.Vector() * Reach;
	return LineTraceEnd;
}
const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	GetPlayerLocRot();

	//	Get play view point this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	
	//	Get Trace by Reach Distance
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
	FHitResult Hit;

	// Setup query params
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	//	Line Trace with other Items
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
		);

	return Hit;
}

/*
*	Gets the Players Location and Rotation in the View Point
*   @returns LocRot PlayerLocRot
*/
LocRot UGrabber::GetPlayerLocRot()
{
	//	Get play view point this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
		);
	LocRot PlayerLocRot = LocRot();
	PlayerLocRot.Location = PlayerViewPointLocation;
	PlayerLocRot.Rotator = PlayerViewPointRotation;

	return PlayerLocRot;
}

//	Find the Physics Handler
void UGrabber::FindPhysicsComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("Physics Actor not found for %s"), *GetOwner()->GetName());
	}
}

//	Find and Bind Input Handler
void UGrabber::FindInputHandler()
{
	InputHandler = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputHandler)
	{
		UE_LOG(LogTemp, Error, TEXT("Found Input Handler for %s"), *GetOwner()->GetName());
		InputHandler->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputHandler->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}
