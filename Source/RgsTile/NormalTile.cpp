// Fill out your copyright notice in the Description page of Project Settings.


#include "NormalTile.h"

void ANormalTile::StepOn()
{
	Super::StepOn();

	MeshComponent->SetVectorParameterValueOnMaterials(FName("TileBaseColor"), FVector(FColor::Silver));
	MeshComponent->SetScalarParameterValueOnMaterials(FName("TileEmission"), 1.0f);

}

void ANormalTile::StepOff()
{
	Super::StepOff();

	MeshComponent->SetVectorParameterValueOnMaterials(FName("TileBaseColor"), FVector(FColor::Silver));
	MeshComponent->SetScalarParameterValueOnMaterials(FName("TileEmission"), 0.0f);
}


void ANormalTile::BeginPlay()
{
	Super::BeginPlay();

	TileType = ETileType::Normal;
}

void ANormalTile::ShowTileColor(bool bShowColor)
{
	if (bShowColor)
	{		
		MeshComponent->SetScalarParameterValueOnMaterials(FName("TileEmission"), 1.0f);
	}
	else
	{
		MeshComponent->SetScalarParameterValueOnMaterials(FName("TileEmission"), 0.0f);
	}
}