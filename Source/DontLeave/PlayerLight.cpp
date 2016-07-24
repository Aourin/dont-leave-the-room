// Fill out your copyright notice in the Description page of Project Settings.

#include "DontLeave.h"
#include "PlayerLight.h"


// Sets default values for this component's properties
UPlayerLight::UPlayerLight()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerLight::BeginPlay()
{
	Super::BeginPlay();

	ParentActor = GetWorld()->GetFirstPlayerController()->GetPawn();
	// ...
	
}


// Called every frame
void UPlayerLight::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
	GetOwner()->SetActorRotation(ParentActor->GetActorRotation());
	// ...
}

