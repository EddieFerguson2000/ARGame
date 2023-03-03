// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackAndy.h"

// Sets default values
AAttackAndy::AAttackAndy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FString greenMaterialName = "/Game/HelloAR/Assets/Materials/Andy_Mat_Default.Andy_Mat_Default";
	ConstructorHelpers::FObjectFinder<UMaterial> greenMaterialAsset(*greenMaterialName);
	AndyMatGreen = UMaterialInstanceDynamic::Create(greenMaterialAsset.Object, this);

	FString blueMaterialName = "/Game/HelloAR/Assets/Materials/Andy_Mat_Blue.Andy_Mat_Blue";
	ConstructorHelpers::FObjectFinder<UMaterial> blueMaterialAsset(*blueMaterialName);
	AndyMat = UMaterialInstanceDynamic::Create(blueMaterialAsset.Object, this);

}

// Called when the game starts or when spawned
void AAttackAndy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAttackAndy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAttackAndy::TakeHit()
{
	
}

