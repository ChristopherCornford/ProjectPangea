//Author Pablo Bengoa | Email: p.bengoanagy@gmail.com


#include "HuntingComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
//Removed later
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UHuntingComponent::UHuntingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

  owning_actor_ = nullptr;
  damage_ = 24.0f;
  range_ = 400.0f;
}


// Called when the game starts
void UHuntingComponent::BeginPlay()
{
  Super::BeginPlay();

  owning_actor_ = Cast<ACharacter>(GetOwner());

  UInputComponent* inputcomp = Cast<UInputComponent>(
    owning_actor_->FindComponentByClass(UInputComponent::StaticClass()));

  if (inputcomp != nullptr) {
    inputcomp->BindAction("Attack", IE_Pressed, this, &UHuntingComponent::MeleeAttack);
  }

  camera_transform_ = Cast<USceneComponent>(
    owning_actor_->FindComponentByClass(UCameraComponent::StaticClass()));
	
}


// Called every frame
void UHuntingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHuntingComponent::MeleeAttack(){
  if (camera_transform_ == nullptr) return;
  
  GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green,
    FString("Attacking"));

  FCollisionQueryParams trace_params =
    FCollisionQueryParams(FName(TEXT("HuntTrace")), true, owning_actor_);
  trace_params.bTraceComplex = true;
  trace_params.bReturnPhysicalMaterial = false;
  trace_params.TraceTag = FName("HuntTrace");


  FHitResult Trace_Hit(ForceInit);
  FVector pos = owning_actor_->GetActorLocation();
  FVector camera_dir = camera_transform_->GetForwardVector();


  bool validHit = GetWorld()->LineTraceSingleByChannel(Trace_Hit,
    pos, pos + (camera_dir * range_),
    ECC_Visibility, trace_params);

  if (debug_mode_)
    DrawDebugLine(GetWorld(), pos, pos + (camera_dir * range_), FColor::Blue, true, 1.0f);

  if (validHit) {
    if (Trace_Hit.GetActor()->ActorHasTag("Prey")) {
      Trace_Hit.GetActor()->TakeDamage(damage_, FDamageEvent(),
        nullptr, owning_actor_);
    }
  }
}