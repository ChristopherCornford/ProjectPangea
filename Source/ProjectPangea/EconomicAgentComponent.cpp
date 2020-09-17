// Fill out your copyright notice in the Description page of Project Settings.


#include "EconomicAgentComponent.h"
#include "ResourceGeneratorComponent.h"
#include "Engine.h"

// Sets default values for this component's properties
UEconomicAgentComponent::UEconomicAgentComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEconomicAgentComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UEconomicAgentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEconomicAgentComponent::BeginJob()
{
	onBeginJob();

	switch (agentRole)
	{
		case AgentRole::Gatherer:
			Gather();
			break;

		case AgentRole::Merchant:
			Trade();
			break;

		case AgentRole::Caravan:
			Transport();
			break;
	}
}

void UEconomicAgentComponent::CompleteJob()
{
	onCompleteJob();

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, TEXT("GATHERING DONE!"));

	this->GetOwner()->SetActorLocation(JobGiver->GetActorLocation());

	UResourceGeneratorComponent* manager = JobGiver->FindComponentByClass<UResourceGeneratorComponent>();

	manager->IdleAgents.Add(this->GetOwner());

	manager->WorkingAgents.Remove(this->GetOwner());

	manager->IdleResourcePoints.Add(JobTarget);

	manager->OccupiedResourcePoints.Remove(JobTarget);
}

void UEconomicAgentComponent::Gather()
{

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, TEXT("GATHERING!"));

}

void UEconomicAgentComponent::Trade()
{

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("TRADING!"));

}

void UEconomicAgentComponent::Transport()
{

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("TRANSPORTING!"));


	this->GetOwner()->SetActorLocation(JobTarget->GetActorLocation());

	this->agentRole = AgentRole::Merchant;

	BeginJob();
}

