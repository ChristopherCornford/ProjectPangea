// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"

#include "LevelStreamerActor.generated.h"


UCLASS()
class PROJECTPANGEA_API ALevelStreamerActor : public AActor
{
	GENERATED_BODY()

private:
    // Overlap volume to trigger level streaming
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UBoxComponent* OverlapVolume;
	
public:	
	// Sets default values for this actor's properties
	ALevelStreamerActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere)
    FName LevelToLoad;

    UFUNCTION()
    void OverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
