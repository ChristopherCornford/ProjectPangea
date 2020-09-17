// Fill out your copyright notice in the Description page of Project Settings.


#include "ResourceGeneratorComponent.h"
#include "GameFramework/Actor.h"
#include "EconomicAgentComponent.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UResourceGeneratorComponent::UResourceGeneratorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UResourceGeneratorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	GiveActorJob(IdleAgents[0]);
}


// Called every frame
void UResourceGeneratorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	if (isReadyToShip && IdleAgents.Num() > 0)
	{
		ShipResources();
	}
}

void UResourceGeneratorComponent::GiveActorJob(AActor* actor)
{
	UEconomicAgentComponent* agent = actor->FindComponentByClass<UEconomicAgentComponent>();

	agent->JobGiver = this->GetOwner();

	WorkingAgents.Add(actor);

	IdleAgents.Remove(actor);

	if (IdleResourcePoints[0])
		agent->JobTarget = IdleResourcePoints[0];

	FVector resourceLocation = IdleResourcePoints[0]->GetActorLocation();

	OccupiedResourcePoints.Add(IdleResourcePoints[0]);

	IdleResourcePoints.RemoveAt(0);

	MoveAgentToResourceLocation(actor, resourceLocation);

	if(IdleAgents.Num() > 0)
		GiveActorJob(IdleAgents[0]);
}

void UResourceGeneratorComponent::MoveAgentToResourceLocation(AActor* agent, FVector location)
{
	UEconomicAgentComponent* agentComp = agent->FindComponentByClass<UEconomicAgentComponent>();

	agent->SetActorLocation(location);

	if (agent->GetActorLocation() == location)
		agentComp->BeginJob();
}

void UResourceGeneratorComponent::ShipResources()
{
	isReadyToShip = false;

	CurrentResourceCache -= QuantityNeededToShip;

	AActor* CaravanActor = IdleAgents[0];

	UEconomicAgentComponent* CaravanAgent = CaravanActor->FindComponentByClass<UEconomicAgentComponent>();

	CaravanAgent->agentRole = AgentRole::Caravan;

	WorkingAgents.Add(CaravanActor);

	IdleAgents.Remove(CaravanActor);

	CaravanAgent->CurrentInventory = TEXT("Ore");

	CaravanAgent->JobTarget = ParentNode;

	CaravanAgent->BeginJob();
}

