// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class PROJECTPANGEA_API Jobs
{
public:
	Jobs();
	virtual ~Jobs();

	static void Gather(AActor* self);
	static void Trade(AActor* self);
};
