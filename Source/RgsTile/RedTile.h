// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Tile.h"

#include "RedTile.generated.h"

class UParticleSystem;

/**
 * 
 */
UCLASS()
class RGSTILE_API ARedTile : public ATile
{
	GENERATED_BODY()
	
public: 
	
	ARedTile();

	virtual void StepOn() override;

	virtual void StepOff() override;

	virtual void ShowTileColor(bool bShowColor) override;

protected:

	virtual void BeginPlay() override;

private: 

	/** Particle system instance to plat with */
	TObjectPtr<UParticleSystem> ParticleSystem;
};
