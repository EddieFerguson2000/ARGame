// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "ARBlueprintLibrary.h"
#include "ARPlaneActor.h"
#include "AndyActor.h"
#include "ARManager.generated.h"

UCLASS()
class AR_STARTER_PROJECT_API AARManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AARManager();
	void ResetARCoreSession();
	void UpdatePlaneActors();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void MoveAndy(FARTraceResult touchPos);
	FHitResult WorldHitTest(FVector2D touchLocation);

	//AActor GetBestAndyActorToReuse();
	AAndyActor* GetBestAndyActorToReuse();
	bool SpawnAndyActor(FARTraceResult hitTestResult);
	bool HasValidPlane();
	FColor GetPlaneColor(int index);
	bool IsHittingFrontSurface(FARTraceResult TraceResult, FVector2D screenPos);

	TArray<AAndyActor*> andyActors;
	TArray<FColor> planeColors;
	int maxSpawnedAndys;
	TMap<UARPlaneGeometry*, AARPlaneActor*> planeActorMap;
	int planeIndex;
	bool selected;
	AAndyActor* selectedAndy;
	//UARSessionConfig* config;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UMaterialInterface* materialToUse;



public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void InputPressed(const ETouchIndex::Type fingerIndex, const FVector screenPos);
	virtual void InputMoved(const ETouchIndex::Type fingerIndex, const FVector screenPos);
	virtual void InputReleased(const ETouchIndex::Type fingerIndex, const FVector screenPos);


};
