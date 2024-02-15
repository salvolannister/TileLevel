// Copyright(c) Forge Reply. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "RgsTileGameMode.generated.h"

class ATile;


UCLASS(minimalapi)
class ARgsTileGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARgsTileGameMode();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	// Resets the game, by forcing the reload of the game level.
	void ResetGame();

	// Returns the total number of Green Tiles in the tile grid.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Game)
	int32 GetTotalGreenTiles();

	// Returns the number of Green Tiles that the player found.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Game)
	int32 GetGreenTilesFound();

	// Returns the total number of Red Tiles in the tile grid.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Game)
	int32 GetTotalRedTiles();

	// Returns the number of Red Tiles that the player found.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Game)
	int32 GetRedTilesFound();

	// Returns the distance from the player to the closest Green Tile (that the player has not found yet).
	// Returns -1 if all Green Tiles have been found.
	// Distance is defined as the number of tiles that the player has to move (horizontally and vertically)
	// to reach the closest Green Tile.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Game)
	int32 GetClosestGreenTileDistance();

	// Returns the distance from the player to the closest Red Tile (that the player has not found yet).
	// Returns -1 if all Red Tiles have been found.
	// Distance is defined as the number of tiles that the player has to move (horizontally and vertically)
	// to reach the closest Red Tile.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Game)
	int32 GetClosestRedTileDistance();

	UPROPERTY(EditAnywhere, Category = "Tiles")
	TSubclassOf<ATile> GreenTileBP;

	UPROPERTY(EditAnywhere, Category = "Tiles")
	TSubclassOf<ATile> RedTileBP;

	UPROPERTY(EditAnywhere, Category = "Tiles")
	TSubclassOf<ATile> BlueTileBP;

	UPROPERTY(EditAnywhere, Category = "Tiles")
	TSubclassOf<ATile> NormalTileBP;

public:

	// Defines the size of the tile grid to generate.
	// Must be at least 3.
	// Can be changed in the "TileGameModeBP" blueprint.
	UPROPERTY(EditDefaultsOnly, Category = Configuration)
	int32 TileGridSize = 10;

	// Defines the number of Green Tiles to randomly place in the tile grid.
	// Must be at least 1.
	// Can be changed in the "TileGameModeBP" blueprint.
	UPROPERTY(EditDefaultsOnly, Category = Configuration, meta = (ClampMin = "1", UIMin = "1"))
	int32 GreenTilesToSpawn = 5;

	// Defines the number of Red Tiles to randomly place in the tile grid.
	// Must be at least 1.
	// Can be changed in the "TileGameModeBP" blueprint.
	UPROPERTY(EditDefaultsOnly, Category = Configuration, meta = (ClampMin = "1", UIMin = "1"))
	int32 RedTilesToSpawn = 5;

	// Dimension of the tile
	UPROPERTY(EditDefaultsOnly, Category = Configuration)
	float SectorSize = 200.f;

private:
	// Generates the tile grid
	void SpawnTileGrid();

	TArray<TArray<TObjectPtr<ATile>>> TileGrid;

	// Debug function to show colored tiles
	void ShowColoredTiles();

	void SpawnRedTiles();

	void SpawnGreenTiles();

	// Checks if a green tile has at least one tile that is not read in it's neighborhood
	bool IsGreenTileReachable(const int32 x, const int32 y) const;


	FVector GetTileLocation(const int32 x, const int32 y);

	FVector TilesGridOffset;
};



