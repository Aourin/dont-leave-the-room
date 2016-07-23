// Fill out your copyright notice in the Description page of Project Settings.

#include "DontLeave.h"
#include "OpenDoor.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

}

//	Returns IsOpen
bool UOpenDoor::GetIsOpen() const
{
	return IsOpen;
}

//	Toggle The Door
void UOpenDoor::ToggleDoor()
{
	if (IsOpen)
	{
		CloseDoor();
	}
	else
	{
		OpenDoor();
	}
}

//	Close The Door
void UOpenDoor::CloseDoor()
{
	AActor* Owner = GetOwner();
	FRotator Rotator = FRotator(0.f, (OpenAngle - OpenAngle), 0.f);
	IsOpen = false;

	Owner->SetActorRotation(Rotator);
}

//	Open The Door
void UOpenDoor::OpenDoor()
{
	AActor* Owner = GetOwner();
	FRotator Rotator = FRotator(0.f, OpenAngle, 0.f);
	IsOpen = true;

	Owner->SetActorRotation(Rotator);
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	OpenDoor();
	// ...
	
}

// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if (PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		if (IsOpen) {
			CloseDoor();
		}
	}
	else {
		if (!IsOpen)
		{
			OpenDoor();
		}
	}
	// ...
}

