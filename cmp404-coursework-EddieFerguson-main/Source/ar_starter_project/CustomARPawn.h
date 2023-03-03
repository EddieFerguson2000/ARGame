// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TimerManager.h" 
#include "ARBlueprintLibrary.h"
#include "ARManager.h"
#include "CustomARPawn.generated.h"

class UCameraComponent;

UCLASS()
class AR_STARTER_PROJECT_API ACustomARPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACustomARPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UARSessionConfig* config;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USceneComponent* ScnComponent;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UCameraComponent* CameraComponent;

	virtual void DisplayCameraInfo();
	virtual void SpawnCube();
	void UpdateTrackingImage();
	virtual void InputPressed(const ETouchIndex::Type fingerIndex, const FVector screenPos);
	virtual void InputMoved(const ETouchIndex::Type fingerIndex, const FVector screenPos);
	virtual void InputReleased(const ETouchIndex::Type fingerIndex, const FVector screenPos);

private:

	FTimerHandle cameraTicker;
	float cameraNotifyLoopTime;
	bool bGoghFound;
	AActor* GoghCube;
	AARManager* manager;

	//trackedImages <UARTrackedImage *>;
};
