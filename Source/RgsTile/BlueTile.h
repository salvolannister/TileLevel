// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tile.h"
#include "BlueTile.generated.h"

/**
 * 
 */
UCLASS()
class RGSTILE_API ABlueTile : public ATile
{
	GENERATED_BODY()

public:
	
	// Method to call to notify when the player steps on this tile.
	virtual void StepOn() override;

	// Method to call to notify when the player steps off this tile.
	virtual void StepOff() override;

	virtual void ShowTileColor(bool bShowColor) override;

	virtual void BeginPlay() override;
};
