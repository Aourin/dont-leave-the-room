// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Grabber.generated.h"

struct LocRot {
	FVector Location;
	FRotator Rotator;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DONTLEAVE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	AActor* TargetActor = nullptr;
	// Called when the game starts
	virtual void BeginPlay() override;

	void FindInputHandler();
	void FindPhysicsComponent();
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	void GetTargetOnRayTrace(FHitResult &Hit);

private:
	UPROPERTY(EditAnywhere)
	float Reach = 100.f;

	UInputComponent* InputHandler = nullptr;
	UPhysicsHandleComponent* PhysicsHandle = nullptr;

	// Raycast and cast in reach
	void Grab();
	void Release();
	const FVector GetReachPoint();
	const FHitResult GetFirstPhysicsBodyInReach();
	LocRot GetPlayerLocRot();
};
