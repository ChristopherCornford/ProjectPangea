// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/InteractuableNPC.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"

AInteractuableNPC::AInteractuableNPC() {

  trigger_ = CreateDefaultSubobject<UBoxComponent>(TEXT("trigger_"));
  trigger_->SetupAttachment(GetCapsuleComponent());
}

void AInteractuableNPC::BeginPlay(){

}

void AInteractuableNPC::Tick(float DeltaTime){

}

bool AInteractuableNPC::Interact_Implementation()
{
  //Do interaction logic (conversation, shop...)
  GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green,
    FString("Interaction Done"));
  return false;
}
