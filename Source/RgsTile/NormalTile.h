// Copyright(c) Forge Reply. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Tile.h"
#include "NormalTile.generated.h"

/**
 * 
 */
UCLASS()
class RGSTILE_API ANormalTile : public ATile
{
	GENERATED_BODY()

public:

	virtual void StepOn() override;

	virtual void StepOff() override;

	virtual void ShowTileColor(bool bShowColor) override;

protected:

	virtual void BeginPlay() override;
};
