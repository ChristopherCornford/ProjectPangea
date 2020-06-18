//Author Pablo Bengoa

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseNPC.generated.h"

UCLASS()
class PROJECTPANGEA_API ABaseNPC : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseNPC();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

  float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
    class AController* EventInstigator, AActor* DamageCauser) override;

};
