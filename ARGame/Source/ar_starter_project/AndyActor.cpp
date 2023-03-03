// Fill out your copyright notice in the Description page of Project Settings.


#include "AndyActor.h"

// Sets default values
AAndyActor::AAndyActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//AndyScnComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Default Scene"));
	//SetRootComponent(AndyScnComponent);

	AndyStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Andy Mesh"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/HelloAR/Assets/Meshes/Andy_Mesh.Andy_Mesh'"));
	//static ConstructorHelpers::FObjectFinder<UMaterial> MeshAsset(TEXT("Material'/Game/HandheldARBP/Materials/M_BackgroundFade.M_BackgroundFade'"));

	AndyStaticMeshComponent->SetStaticMesh(MeshAsset.Object);
	AndyStaticMeshComponent->SetupAttachment(RootComponent);

	/*FString greenMaterialName = "/Game/HelloAR/Assets/Materials/Andy_Mat_Default.Andy_Mat_Default";
	ConstructorHelpers::FObjectFinder<UMaterial> greenMaterialAsset(*greenMaterialName);
	AndyMatGreen = UMaterialInstanceDynamic::Create(greenMaterialAsset.Object, this);

	FString blueMaterialName = "/Game/HelloAR/Assets/Materials/Andy_Mat_Blue.Andy_Mat_Blue";
	ConstructorHelpers::FObjectFinder<UMaterial> blueMaterialAsset(*blueMaterialName);
	AndyMatBlue = UMaterialInstanceDynamic::Create(blueMaterialAsset.Object, this);*/

}

// Called when the game starts or when spawned
void AAndyActor::BeginPlay()
{
	Super::BeginPlay();


}

// Called every frame
void AAndyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*if (selected)
	{
		AndyStaticMeshComponent->SetMaterial(0, AndyMatGreen);
	}
	else
	{
		AndyStaticMeshComponent->SetMaterial(0, AndyMatBlue);
	}*/

	if (IsValid(CARPin))
	{
		//AndyScnComponent->SetVisibility((CARPin->GetTrackingState() == EARTrackingState::Tracking), true);

		if (CARPin->GetTrackingState() == EARTrackingState::Tracking)
		{
			SetActorTransform(CARPin->GetLocalToWorldTransform());
		}
		else if (CARPin->GetTrackingState() == EARTrackingState::StoppedTracking)
		{
			CARPin = nullptr;
		}
	}
	else
	{
		//	AndyScnComponent->SetVisibility(false, false);

	}

}

void AAndyActor::CAssignARPin(UARPin* NewARPin)
{
	if (IsValid(CARPin))
	{
		UARBlueprintLibrary::RemovePin(CARPin);
		CARPin = NewARPin;
	}
	else
	{
		CARPin = NewARPin;
	}

}

bool AAndyActor::HasValidARPin()
{
	return IsValid(CARPin);

}

void AAndyActor::SetSelected(bool newBool)
{
	selected = newBool;
}