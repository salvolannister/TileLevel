// Copyright(c) Forge Reply. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Tile.generated.h"


UENUM()
enum class ETileType : uint8
{
	Normal,
	Green,
	Red,
	Blue
};

class UTextRenderComponent;

// Implementation of the Tile class from which other Tile class should derive from.
// It consist in a platform sized 2m x 2m x 1m.
UCLASS()
class RGSTILE_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	ATile();

protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

	// Sets the type of this tile.
	void SetType(ETileType Type);

	// Returns the type of this tile.
	ETileType GetType() const;

	// Method to call to notify when the player steps on this tile.
	virtual void StepOn();

	// Method to call to notify when the player steps off this tile.
	virtual void StepOff();

	// Debug: Writes the coordinates x and y on the corresponding tile on the TileCoordinateText
	void SetRenderText(const int32 x, const int32 y);

	// Show tile color without visiting it 
	virtual void ShowTileColor(bool bShowColor);

	// Tells if the player visited or not the tile
	virtual bool HasBeenVisited() const;

	// Stores the grid position of the tile so we can retrieve it when we an ATile*.
	void StoreTileGridPosition(const int32 x, const int32 y);
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Visual, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComponent;

	// X position of the tile inside the grid, values should be (0, TileGridSize - 1)
	UPROPERTY(VisibleAnywhere)
	int32 TilePosX;
	// Y position of the tile inside the grid, values should be (0, TileGridSize - 1)
	UPROPERTY(VisibleAnywhere)
	int32 TilePosY;

protected:

	UPROPERTY(EditAnywhere)
	ETileType TileType = ETileType::Normal;

	// Text Render Component for displaying debugging information above the tile.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Visual, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTextRenderComponent> TileCoordinateText;

	// Indicates whether the tile has been visited or not.
	bool bVisited = false;
	

};
