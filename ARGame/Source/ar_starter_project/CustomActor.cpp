// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomActor.h"

// Sets default values
ACustomActor::ACustomActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Setup Scene COmponent as default
	ScnComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Default Scene"));
	SetRootComponent(ScnComponent);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));

	// Get mesh from Unreal's Reference  Manager. (Right click on object and Get Reference"
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	//static ConstructorHelpers::FObjectFinder<UMaterial> MeshAsset(TEXT("Material'/Game/HandheldARBP/Materials/M_BackgroundFade.M_BackgroundFade'"));

	StaticMeshComponent->SetStaticMesh(MeshAsset.Object);
	StaticMeshComponent->SetupAttachment(RootComponent);


}

// Called when the game starts or when spawned
void ACustomActor::BeginPlay()
{
	//Super::BeginPlay();
	

}


void ACustomActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();


	initLocation = GetActorLocation();


}


// Called every frame
void ACustomActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Get local position based on Infinity Path
	t += DeltaTime*3;			// Here we are accumulating delta time but this also accumulates floating point errors. Instead of this method, use the FTime class in unreal.
	double x, y;
	double scale = 2 / (3 - cos(2 * t));
	x = scale * cos(t);
	y = scale * sin(2 * t) / 2;


	// Set actor location
	FVector relativeLoc = FVector4(0, x * 100, y * 100);



	//Set Scale matrix
	FScaleMatrix mat_scale(0.3);

	// 1	0	0	0
	// 0	1	0	0
	// 0	0	1	0
	// X	Y	Z	1 


	// Set local Translation Matrix
	FTranslationMatrix mat_local(relativeLoc);

	// Set Translation matrix with spawn Point
	FTranslationMatrix mat_init(initLocation);

	auto mat_final = mat_scale * mat_local * mat_init;
	SetActorTransform(FTransform(mat_final));

}

