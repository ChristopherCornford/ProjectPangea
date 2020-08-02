// Fill out your copyright notice in the Description page of Project Settings.


#include "RangedCombat.h"

// Sets default values for this component's properties
URangedCombat::URangedCombat()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void URangedCombat::BeginPlay()
{
	Super::BeginPlay();

	MainCamera = Cast<UCameraComponent>(GetWorld()->GetFirstPlayerController()->GetPawn()->
		GetComponentsByTag(UCameraComponent::StaticClass(), TEXT("MainCamera"))[0]);
	//MainCameraBoom = Cast<USpringArmComponent>(GetWorld()->GetFirstPlayerController()->GetPawn()->
	//	GetComponentsByTag(USpringArmComponent::StaticClass(), TEXT("MainCameraBoom"))[0]);
	ADSCamera = Cast<UCameraComponent>(GetWorld()->GetFirstPlayerController()->GetPawn()->
		GetComponentsByTag(UCameraComponent::StaticClass(), TEXT("ADSCamera"))[0]);
	MainCamPos = MainCamera->GetRelativeLocation();
	MainCamRot = MainCamera->GetRelativeRotation();
	ADSPos = ADSCamera->GetRelativeLocation();
	ADSRot = ADSCamera->GetRelativeRotation();
	ADSCamera->SetActive(false);

	//CrosshairHUD = CreateWidget<UUserWidget>(GetWorld(), UUserWidget::StaticClass(), TEXT("CrosshairHUD_Blueprint"));
}


// Called every frame
void URangedCombat::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed("leftshift"))
	{
		MainCamera->SetRelativeLocation(ADSPos);
		MainCamera->SetRelativeRotation(ADSRot);
		//MainCamera->SetActive(false);
		//ADSCamera->SetActive(true);
		
		CrosshairHUD->AddToViewport();

		IsAiming = true;
	}
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustReleased("leftshift"))
	{
		MainCamera->SetRelativeLocation(MainCamPos);
		MainCamera->SetRelativeRotation(MainCamRot);
		//ADSCamera->SetActive(false);
		//MainCamera->SetActive(true);
		
		CrosshairHUD->RemoveFromViewport();

		IsAiming = false;
	}

	if (IsAiming)
	{
		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed("Z"))
		{
			FireProjectile();
		}
		//TO DO: Make sure character is always facing the same way as the camera.
	}

	if (FiredProjectileList.Num() > 0)
	{
		for (int i = 0; i < FiredProjectileList.Num(); i++)
		{
			if (FiredProjectileList[i]->GetGameTimeSinceCreation() > 5.0f)
			{
				FiredProjectileList[i]->Destroy();
				FiredProjectileList.RemoveAt(i);
				break;
			}
		}
	}
}

void URangedCombat::FireProjectile()
{
	FVector SpawnLocation = GetOwner()->GetActorLocation() + (70.0f * MainCamera->GetForwardVector())
		+ (15.0f * MainCamera->GetRightVector()) + (50.0f * FVector(0.0f, 0.0f, 1.0f));
	FRotator SpawnRotation = GetOwner()->GetActorRotation();
	AActor* SpawnedProjectile = GetWorld()->SpawnActor(ProjectileToSpawn, &SpawnLocation, &SpawnRotation);
	FiredProjectileList.Add(SpawnedProjectile);
	SpawnedProjectile->FindComponentByClass<UMeshComponent>()->SetSimulatePhysics(true);
	float ForceStrength = 100000.0f;//15000.0f;
	FVector ForceVector = MainCamera->GetForwardVector();
	//ForceVector.Z = 1.0f;
	ForceVector = ForceVector * ForceStrength * SpawnedProjectile->FindComponentByClass<UStaticMeshComponent>()->GetMass();
	SpawnedProjectile->FindComponentByClass<UStaticMeshComponent>()->AddForce(ForceVector);
}