// Copyright(c) Forge Reply. All Rights Reserved.


#include "GreenTile.h"

void AGreenTile::StepOn()
{
	Super::StepOn();

	if (bVisited)
		return;

	bVisited = true;
	ShowTileColor(true);
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