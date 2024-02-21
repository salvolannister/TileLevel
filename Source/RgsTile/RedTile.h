// Copyright(c) Forge Reply. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>

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

	/* Time after which you would like to make the tile turn red after the special effect is spawn*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Configuration", meta = (ClampMin = "0", UIMin = "0"))
	float DelayTimeInSeconds = 0.3f; 

protected:

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private: 

    /* Used to enable showing the effect before the change of state */
	UFUNCTION()
	void UpdateTileStateToVisitedDelayed();

	FTimerHandle UpdateTileToVisitedTimerHandle;

};
