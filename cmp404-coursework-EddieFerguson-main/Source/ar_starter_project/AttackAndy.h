// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AttackAndy.generated.h"

UCLASS()
class AR_STARTER_PROJECT_API AAttackAndy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAttackAndy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void TakeHit();
	int health;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* AndyStaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UMaterialInstance* AndyMat;
};
