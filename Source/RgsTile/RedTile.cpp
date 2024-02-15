// Fill out your copyright notice in the Description page of Project Settings.


#include "RedTile.h"


void ARedTile::StepOn()
{
	ATile::StepOn();

	if (bIsVisited)
		return;
	
	MeshComponent->SetVectorParameterValueOnMaterials(FName("TileBaseColor"), FVector(FColor::Red));
	MeshComponent->SetScalarParameterValueOnMaterials(FName("TileEmission"), 1.0f);
	
}

void ARedTile::StepOff()
{
	Super::StepOff();
}


void ARedTile::BeginPlay()
{
	Super::BeginPlay();

	TileType =  ETileType::Red;
}