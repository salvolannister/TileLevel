// Copyright(c) Forge Reply. All Rights Reserved.


#include "BlueTile.h"

void ABlueTile::StepOn()
{
	Super::StepOn();

	bVisited = true;

	ShowTileColor(true);
}

void ABlueTile::StepOff()
{
	Super::StepOff();

	ShowTileColor(false);
}

void ABlueTile::ShowTileColor(bool bShowColor)
{
	if (bShowColor)
	{
		MeshComponent->SetVectorParameterValueOnMaterials(FName("TileBaseColor"), FVector(FColor::Blue));
		MeshComponent->SetScalarParameterValueOnMaterials(FName("TileEmission"), 1.0f);
	}
	else
	{
		MeshComponent->SetVectorParameterValueOnMaterials(FName("TileBaseColor"), FVector(FColor::Silver));
		MeshComponent->SetScalarParameterValueOnMaterials(FName("TileEmission"), 0.f);
	}
}
