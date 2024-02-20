// Fill out your copyright notice in the Description page of Project Settings.


#include "RedTile.h"

#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h"

void ARedTile::StepOn()
{
	ATile::StepOn();

	if (bVisited)
		return;

	if (ParticleSystem)
	{
		FVector ParticlePosition = GetActorLocation();
		ParticlePosition.Z += 10.f;
		UParticleSystemComponent* ParticleComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleSystem, ParticlePosition, FRotator::ZeroRotator, true);
		ParticleComponent->OnParticleDeath.AddDynamic(this, &ARedTile::HandleParticleDeath);
	}


	
}

void ARedTile::StepOff()
{
	Super::StepOff();
}


void ARedTile::BeginPlay()
{
	Super::BeginPlay();

	TileType =  ETileType::Red;

	bVisited = false;
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

void ARedTile::HandleParticleDeath(FName EventName, float EmitterTime, int32 ParticleTime, FVector Location, FVector Velocity, FVector Direction)
{
	bVisited = true;
	ShowTileColor(true);
}