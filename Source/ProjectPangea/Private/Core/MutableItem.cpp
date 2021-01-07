// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/MutableItem.h"

// Sets default values
AMutableItem::AMutableItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AMutableItem::BeginPlay()
{
	Super::BeginPlay();
	
}

bool AMutableItem::Use_Implementation() {
  return false;
}

FItemData AMutableItem::getItemData(){
  return FItemData();
}

void AMutableItem::setItemData(FItemData data){
}

