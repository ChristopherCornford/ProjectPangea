// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "StreamingVolumesData.h"

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
    
    UPROPERTY(EditAnywhere)
    FName LevelToLoad;
    UPROPERTY(EditAnywhere)
    FName LevelToUnload;

    bool isInZone;
    UPROPERTY(EditAnywhere)
    AStreamingVolumesData * streamingVolumeData;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    UFUNCTION()
    void OverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
    UFUNCTION()
    void OverlapEnds(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    void unload(FName levelToUnload);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
