// Fill out your copyright notice in the Description page of Project Settings.


#include "ARManager.h"

// Sets default values
AARManager::AARManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	maxSpawnedAndys = 10;
	//static ConstructorHelpers::FObjectFinder<UARSessionConfig> ConfigAsset(TEXT("ARSessionConfig'/Game/Images/NewDataAsset.NewDataAsset'"));
	//config = ConfigAsset.Object;

}

// Called when the game starts or when spawned
void AARManager::BeginPlay()
{
	//UARBlueprintLibrary::StartARSession(config);
	Super::BeginPlay();


}

// Called every frame
void AARManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void AARManager::MoveAndy(FARTraceResult touchPos)
{
	UARPin* uarPin = UARBlueprintLibrary::PinComponent(0, touchPos.GetLocalToWorldTransform(), touchPos.GetTrackedGeometry());

	selectedAndy->CAssignARPin(uarPin);

}

FHitResult AARManager::WorldHitTest(FVector2D touchLocation)
{
	FHitResult hitR(ForceInit);
	FVector worldPos, worldDir, endPos;
	APlayerController* pController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	pController->DeprojectScreenPositionToWorld(touchLocation.X, touchLocation.Y, worldPos, worldDir);

	worldDir *= 1000.f;
	endPos = worldPos + worldDir;

	//Creating an array of object types to trace for
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypesArray;
	objectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
	objectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
	objectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	objectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECC_PhysicsBody));
	objectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECC_Vehicle));
	objectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECC_Destructible));

	GetWorld()->LineTraceSingleByObjectType(hitR, worldPos, endPos, objectTypesArray);

	if (hitR.GetActor() != nullptr)
	{
		UKismetSystemLibrary::PrintString(this, FString(TEXT("Hit Detected")), true, true, FLinearColor(0.7, 1, 0, 1), 5);
	}
	else
	{
		UKismetSystemLibrary::PrintString(this, FString(TEXT("Undetected")), true, true, FLinearColor(0.7, 1, 0, 1), 5);

	}

	return hitR;
}

void AARManager::ResetARCoreSession()
{
	for (int i = 0; i < andyActors.Num(); i++)
	{
		andyActors[i]->Destroy();
	}
	andyActors.Empty();

	planeActorMap.Empty();
}

AAndyActor* AARManager::GetBestAndyActorToReuse()
{
	AAndyActor* returnVal;
	for (int i = 0; i < andyActors.Num(); i++)
	{
		int lastIndex, getElem;
		for (int j = 0; j < andyActors.Num(); j++)
		{
			if (andyActors.IsValidIndex(j))
			{
				lastIndex = j;
			}
		}

		getElem = lastIndex - i;

		if (!andyActors[getElem]->HasValidARPin())
		{
			returnVal = andyActors[getElem];
			andyActors.RemoveAt(getElem);
			return returnVal;
		}
	}

	returnVal = andyActors[0];
	andyActors.Add(returnVal);
	andyActors.RemoveAt(0);
	return returnVal;

	//AActor pog;
	//return pog;
}

bool AARManager::SpawnAndyActor(FARTraceResult hitTestResult)
{
	UARPin* newPin = UARBlueprintLibrary::PinComponent(0, hitTestResult.GetLocalToWorldTransform(), hitTestResult.GetTrackedGeometry());

	if (IsValid(newPin))
	{
		UARTrackedPoint* trackedPoint = Cast<UARTrackedPoint>(hitTestResult.GetTrackedGeometry());
		if (trackedPoint != nullptr)
		{
			FString blueMaterialName = "/Game/HelloAR/Assets/Materials/Andy_Mat_Default.Andy_Mat_Default";
			static ConstructorHelpers::FObjectFinder<UMaterial> blueMaterialAsset(*blueMaterialName);
			materialToUse = blueMaterialAsset.Object;
		}
		else
		{
			FString greenMaterialName = "/Game/HelloAR/Assets/Materials/Andy_Mat_Default.Andy_Mat_Default";
			static ConstructorHelpers::FObjectFinder<UMaterial> greenMaterialAsset(*greenMaterialName);
			materialToUse = greenMaterialAsset.Object;
		}

		if (andyActors.Num() < maxSpawnedAndys)
		{
			FActorSpawnParameters SpawnInfo;
			FTransform myTransform = newPin->GetLocalToWorldTransform();
			AAndyActor* newAndy = GetWorld()->SpawnActor<AAndyActor>(myTransform.GetLocation(), myTransform.GetRotation().Rotator(), SpawnInfo);
			//Actor spawned
			UKismetSystemLibrary::PrintString(this, FString(TEXT("Actor spawned")), true, true, FLinearColor(0, 0.66, 1, 1), 5);

			newAndy->AndyStaticMeshComponent->SetMaterial(0, materialToUse);
			newAndy->CAssignARPin(newPin);
			andyActors.Add(newAndy);
			return true;
		}
		else
		{
			AAndyActor* bestAndy = GetBestAndyActorToReuse();
			bestAndy->AndyStaticMeshComponent->SetMaterial(0, materialToUse);
			bestAndy->CAssignARPin(newPin);
			return true;
		}

	}
	else
	{
		return false;
	}
}

bool AARManager::HasValidPlane()
{
	for (const TPair<UARPlaneGeometry*, AARPlaneActor*>& pair : planeActorMap)
	{
		if (pair.Key->GetTrackingState() == EARTrackingState::Tracking)
		{
			return true;
		}
	}

	return false;
}

void AARManager::UpdatePlaneActors()
{

	TArray<UARPlaneGeometry*> planes = UARBlueprintLibrary::GetAllGeometriesByClass<UARPlaneGeometry>();
	int planeID = 0;
	//UARPlaneGeometry* updatedPlane;
	EARTrackingState planeState;

	for (auto updatedPlane : planes)
	{
		//updatedPlane = planes[i];
		planeState = updatedPlane->GetTrackingState();


		if (planeActorMap.Find(updatedPlane))
		{
			AARPlaneActor* currentPlane = *planeActorMap.Find(updatedPlane);

			if (IsValid(updatedPlane->GetSubsumedBy()))
			{
				currentPlane->Destroy();
				planeActorMap.Remove(updatedPlane);
			}
			else
			{
				if (planeState == EARTrackingState::Tracking)
				{
					currentPlane->UpdatePlanePolygonMesh();
					//UKismetSystemLibrary::PrintString(this, FString(TEXT("Plane Updated")), true, true, FLinearColor(0.7, 1, 0, 1), 5);
				}
				else if (planeState == EARTrackingState::StoppedTracking)
				{
					currentPlane->Destroy();
					planeActorMap.Remove(updatedPlane);
				}
			}
		}
		else
		{
			if (planeState == EARTrackingState::Tracking)
			{
				if (!IsValid(updatedPlane->GetSubsumedBy()))
				{
					FActorSpawnParameters SpawnInfo;
					FVector planeLoc(0, 0, 0);
					FRotator planeRot(0, 0, 0);
					FVector planeScale(1, 1, 1);
					AARPlaneActor* newPlane = GetWorld()->SpawnActor<AARPlaneActor>(planeLoc, planeRot, SpawnInfo);

					//newPlane->SetColor(GetPlaneColor(planeID));
					newPlane->ARCorePlaneObject = updatedPlane;
					planeActorMap.Add(updatedPlane, newPlane);
					UKismetSystemLibrary::PrintString(this, FString(TEXT("New plane added")), true, true, FLinearColor(0, 0.66, 1, 1), 5);
					newPlane->UpdatePlanePolygonMesh();
					//planeID++;
				}
			}
		}

	}
}

FColor AARManager::GetPlaneColor(int index)
{
	int mod = index % planeColors.Num();
	return planeColors[mod];
}

bool AARManager::IsHittingFrontSurface(FARTraceResult traceResult, FVector2D screenPos)
{
	FRotator rot = traceResult.GetLocalToWorldTransform().Rotator();
	FVector upVec = rot.Vector().UpVector;
	FVector worldDir, worldPos;
	APlayerController* plyr = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	plyr->DeprojectScreenPositionToWorld(screenPos.X, screenPos.Y, worldPos, worldDir);
	return (upVec.DotProduct(upVec, worldDir) < 0);


}


void AARManager::InputPressed(const ETouchIndex::Type fingerIndex, const FVector screenPos)
{
	//APlayerController* plyr = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	AActor* hitActor = WorldHitTest(FVector2D(screenPos.X, screenPos.Y)).GetActor();
	if (hitActor != nullptr)
	{
		selected = true;
		for (int i = 0; i < andyActors.Num(); i++)
		{
			if (hitActor == (andyActors[i]))
			{
				if (IsValid(selectedAndy))
				{
					selectedAndy->SetSelected(false);
				}
				selectedAndy = andyActors[i];
				selectedAndy->SetSelected(true);
			}
		}
	}
	else
	{
		if (IsValid(selectedAndy))
		{
			selectedAndy->SetSelected(false);
			selectedAndy = nullptr;
		}
		TArray<FARTraceResult> hits = UARBlueprintLibrary::LineTraceTrackedObjects(FVector2D(screenPos.X, screenPos.Y), false, false, false, true);
		if (hits.IsValidIndex(0))
		{
			if (IsHittingFrontSurface(hits[0], FVector2D(screenPos.X, screenPos.Y)))
			{
				SpawnAndyActor(hits[0]);
				//{
					//printf("Failed to create ARPin from Hit Result");
				//}
			}
		}
	}
}

void AARManager::InputMoved(const ETouchIndex::Type fingerIndex, const FVector screenPos)
{
	if (selected)
	{
		TArray<FARTraceResult> hits = UARBlueprintLibrary::LineTraceTrackedObjects(FVector2D(screenPos.X, screenPos.Y), false, false, false, true);
		MoveAndy(hits[0]);
	}
}

void AARManager::InputReleased(const ETouchIndex::Type fingerIndex, const FVector screenPos)
{

}
