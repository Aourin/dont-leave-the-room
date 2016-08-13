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
	OnCloseRequest.Broadcast();
}

//	Open The Door
void UOpenDoor::OpenDoor()
{	
	OnOpenRequest.Broadcast();
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{	
	Super::BeginPlay();
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Pressure Plate found for %s"), *GetOwner()->GetName());
	} else if (!ActorThatOpens)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Actor that Opens found for %s"), *GetOwner()->GetName());
	}
	else if (!LightPressurePlate)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Light Pressure Plate that Opens found for %s"), *GetOwner()->GetName());
	}
	else {
		OpenDoor();
	}
	// ...
	
}

// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
	if (!PressurePlate)
	{
		return;
	}
	if (!ActorThatOpens)
	{
		return;
	}
	if (!LightPressurePlate)
	{
		return;
	}

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
	if (!LightPressurePlate)
	{
		return 0.f;
	}
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