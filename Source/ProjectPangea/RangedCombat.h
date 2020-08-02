// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/StaticMeshComponent.h"
//#include "GameFramework/SpringArmComponent.h"
#include "RangedCombat.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTPANGEA_API URangedCombat : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URangedCombat();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	bool IsAiming = false;

	UCameraComponent* MainCamera;
	UCameraComponent* ADSCamera;
	//USpringArmComponent* MainCameraBoom;
	FVector MainCamPos;
	FRotator MainCamRot;
	FVector ADSPos;
	FRotator ADSRot;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> ProjectileToSpawn;

	UPROPERTY(EditAnywhere)
		UUserWidget* CrosshairHUD;

	TArray<AActor*> FiredProjectileList;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void FireProjectile();
};
