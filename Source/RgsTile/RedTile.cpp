// Fill out your copyright notice in the Description page of Project Settings.


#include "RedTile.h"

#include "Particles/ParticleSystem.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h"

ARedTile::ARedTile()
{

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ExplosionParticles(TEXT("/Game/StarterContent/Particles/P_Explosion"));
	if (ExplosionParticles.Object)
	{
		ParticleSystem = ExplosionParticles.Object;
	}

}

void ARedTile::StepOn()
{
	ATile::StepOn();

	if (bVisited)
		return;
	
	

	if (ParticleSystem)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleSystem, GetActorLocation(), FRotator::ZeroRotator, true);
	}

	bVisited = true;
	ShowTileColor(true);
	
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