// Fill out your copyright notice in the Description page of Project Settings.


#include "GreenTile.h"

void AGreenTile::StepOn()
{
	Super::StepOn();

	if (bVisited)
		return;

	bVisited = true;
	MeshComponent->SetVectorParameterValueOnMaterials(FName("TileBaseColor"), FVector(FColor::Green));
	MeshComponent->SetScalarParameterValueOnMaterials(FName("TileEmission"), 1.0f);

}

void AGreenTile::StepOff()
{
	Super::StepOff();
}


void AGreenTile::BeginPlay()
{
	Super::BeginPlay();

	TileType = ETileType::Green;
	bVisited = false;
}


void AGreenTile::ShowTileColor(bool bShowColor)
{
	if (bShowColor)
	{
		MeshComponent->SetVectorParameterValueOnMaterials(FName("TileBaseColor"), FVector(FColor::Green));
		MeshComponent->SetScalarParameterValueOnMaterials(FName("TileEmission"), 1.0f);
	}
	else
	{
		MeshComponent->SetVectorParameterValueOnMaterials(FName("TileBaseColor"), FVector(FColor::Silver));
		MeshComponent->SetScalarParameterValueOnMaterials(FName("TileEmission"), 0.0f);
	}
}