// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tile.h"
#include "RedTile.generated.h"

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

protected:

	virtual void BeginPlay() override;

};
