// Fill out your copyright notice in the Description page of Project Settings.


#include "Triceratops_Base_Test.h"

ATriceratops_Base_Test::ATriceratops_Base_Test() {

	fsSpeciesName = "Triceratops";

	nGenderSelector = FMath::RandRange(0, 1);
	if (nGenderSelector == 0) {
		Gender = eGender::Male;
	}
	else {
		Gender = eGender::Female;
	}
	DinoType = eDinoType::LandHerbivore;

	nLevel = FMath::RandRange(1, 15);

	fDinoHealth = (nLevel * 50) + (nLevel * 1);

	fDinoStamina = fDinoHealth / 3.0f;

	fDinoAttack = 50 + (nLevel * 1.2f);

	fDinoDefense = 150 + (nLevel * 1.5f);
}