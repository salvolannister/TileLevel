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

	virtual void StepOn() override;

	virtual void StepOff() override;

	virtual void ShowTileColor(bool bShowColor) override;

	/** Particle system instance to plat with */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
	TObjectPtr<UParticleSystem> ParticleSystem;

protected:

	virtual void BeginPlay() override;

private: 

	UFUNCTION(BlueprintCallable)
	void HandleParticleDeath(FName EventName, float EmitterTime, int32 ParticleTime, FVector Location, FVector Velocity, FVector Direction);

};
