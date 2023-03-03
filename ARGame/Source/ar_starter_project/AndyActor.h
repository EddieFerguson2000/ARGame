// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARBlueprintLibrary.h"
#include "ARPin.h"
#include "AndyActor.generated.h"

UCLASS()
class AR_STARTER_PROJECT_API AAndyActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAndyActor();
	void CAssignARPin(UARPin* NewARPin);
	bool HasValidARPin();
	void SetSelected(bool newBool);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//void CAssignARPin(UARPin* NewARPin);

	UARPin* CARPin;
	bool selected;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	//	USceneComponent* AndyScnComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* AndyStaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UMaterialInstance* AndyMatGreen;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UMaterialInstance* AndyMatBlue;

};
