// Copyright(c) Forge Reply. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>

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

	virtual void ShowTileColor(bool bShowColor) override;

};
