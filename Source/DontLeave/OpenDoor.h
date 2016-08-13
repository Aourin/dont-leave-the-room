// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOpenRequest);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCloseRequest);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DONTLEAVE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();
	bool GetIsOpen() const;
	void ToggleDoor();
	void OpenDoor();
	void CloseDoor();
	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;
	float GetTotalMassOfActorsOnPlate();

	UPROPERTY(BlueprintAssignable)
	FOnOpenRequest OnOpenRequest;

	UPROPERTY(BlueprintAssignable)
	FOnCloseRequest OnCloseRequest;

private:
	UPROPERTY(EditAnywhere)
	bool IsOpen = false;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* LightPressurePlate = nullptr;

	UPROPERTY(EditAnywhere)
	float TriggerWeight = 32.f;
	bool ShouldOpen(float SomeWeight);
	AActor* ActorThatOpens = nullptr;

};
