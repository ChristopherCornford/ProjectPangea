//Author Pablo Bengoa | Email: p.bengoanagy@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CreatureTest.generated.h"

UCLASS()
class PROJECTPANGEA_API ACreatureTest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACreatureTest();

  UPROPERTY(VisibleAnywhere, category = "Components")
    UStaticMeshComponent* sm_comp;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Settings")
    float health_;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

  float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
    class AController* EventInstigator, AActor* DamageCauser) override;
};
