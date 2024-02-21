// Copyright(c) Forge Reply. All Rights Reserved.


#include "RedTile.h"

#include <Particles/ParticleSystem.h>
#include <Particles/ParticleSystemComponent.h>
#include <Kismet/GameplayStatics.h>

void ARedTile::StepOn()
{
	ATile::StepOn();

	if (bVisited)
		return;


	if (ParticleSystem)
	{
		FVector ParticlePosition = GetActorLocation();
		ParticlePosition.Z += 20.f;
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleSystem, ParticlePosition, FRotator::ZeroRotator, true);
		GetWorldTimerManager().SetTimer(UpdateTileToVisitedTimerHandle, this, &ARedTile::UpdateTileStateToVisitedDelayed, DelayTimeInSeconds, false);

	}
	
}


void ARedTile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// Ensure to clear the timer handle to avoid calling functions on a potentially destroyed object
	GetWorldTimerManager().ClearTimer(UpdateTileToVisitedTimerHandle);
}

void ARedTile::ShowTileColor(bool bShowColor)
{
	if (bShowColor)
	{
		MeshComponent->SetVectorParameterValueOnMaterials(FName("TileBaseColor"), FVector(FColor::Red));
		MeshComponent->SetScalarParameterValueOnMaterials(FName("TileEmission"), 1.0f);
	}
	else
	{
		MeshComponent->SetVectorParameterValueOnMaterials(FName("TileBaseColor"), FVector(FColor::Silver));
		MeshComponent->SetScalarParameterValueOnMaterials(FName("TileEmission"), 0.0f);
	}
}


void ARedTile::UpdateTileStateToVisitedDelayed()
{
	ShowTileColor(true);
	bVisited = true;

}