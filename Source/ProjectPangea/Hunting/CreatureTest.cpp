//Author Pablo Bengoa | Email: p.bengoanagy@gmail.com


#include "CreatureTest.h"

// Sets default values
ACreatureTest::ACreatureTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

  sm_comp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("sm_comp"));
  RootComponent = sm_comp;

  health_ = 100.0f;
}

// Called when the game starts or when spawned
void ACreatureTest::BeginPlay()
{
	Super::BeginPlay();
	
  GetWorldTimerManager()->SetTimet()

}

// Called every frame
void ACreatureTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ACreatureTest::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
  class AController* EventInstigator, AActor* DamageCauser) {

  health_ -= DamageAmount;

  if (health_ <= 0.0f) Destroy();
  GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green,
    FString("Health: ") + FString::SanitizeFloat(health_));

  return 0.0f;
}

