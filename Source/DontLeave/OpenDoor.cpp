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
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	OpenDoor();
	// ...
	
}

// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if (PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		float TotalMass = GetTotalMassOfActorsOnPlate();
		UE_LOG(LogTemp, Warning, TEXT("%f is the mass"), TotalMass);
		if (ShouldOpen(TotalMass))
		{
			OpenDoor();
		} else {
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

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;

	TArray<AActor*> OverlappingActors;
	LightPressurePlate->GetOverlappingActors(OUT OverlappingActors);

	for (const auto& Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%f on pressure plate"), TotalMass);
	}

	return TotalMass;
}

bool UOpenDoor::ShouldOpen(float TotalMass)
{
	return TotalMass == TriggerWeight;
}