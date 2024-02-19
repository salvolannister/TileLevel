// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tile.h"
#include "GreenTile.generated.h"

/**
 * 
 */
UCLASS()
class RGSTILE_API AGreenTile : public ATile
{
	GENERATED_BODY()

public:

	virtual void StepOn() override;

	virtual void StepOff() override;

	virtual void ShowTileColor(bool bShowColor) override;

protected:

	virtual void BeginPlay() override;

};
