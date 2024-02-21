// Copyright(c) Forge Reply. All Rights Reserved.


#include "NormalTile.h"

void ANormalTile::StepOn()
{
	Super::StepOn();

	ShowTileColor(true);

}

void ANormalTile::StepOff()
{
	Super::StepOff();

	ShowTileColor(false);
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