// Copyright(c) Forge Reply. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/Actor.h"

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
	UFUNCTION(BlueprintCallable)
	void ResetGame();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTEndGameMulticastDelegate, bool, bIsWin, bool, bForceRestart);

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

	/** The blueprint class to be instantiated for the green tile. Set this in the game mode blueprint	*/
	UPROPERTY(EditAnywhere, Category = "Tiles")
	TSubclassOf<ATile> GreenTileBP;

	/** The blueprint class to be instantiated for the red tile. Set this in the game mode blueprint */
	UPROPERTY(EditAnywhere, Category = "Tiles")
	TSubclassOf<ATile> RedTileBP;

	/** The blueprint class to be instantiated for the blue tile. Set this in the game mode blueprint */
	UPROPERTY(EditAnywhere, Category = "Tiles")
	TSubclassOf<ATile> BlueTileBP;

	/** The blueprint class to be instantiated for the normal tile. Set this in the game mode blueprint */
	UPROPERTY(EditAnywhere, Category = "Tiles")
	TSubclassOf<ATile> NormalTileBP;

	/** Delegate used for subscribing to notifications when the game ends. */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Event")
	FTEndGameMulticastDelegate OnEndGameDelegate;

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

	// Used to access quickly to a tile given a (x, y) coordinate deduced from the player position
	TArray<TArray<TObjectPtr<ATile>>> TileGrid;

	// Debug function to show colored tiles
	void ShowColoredTiles();

	// Generates red tiles on the grid and stores them also in an array
	void SpawnRedTiles();

	// Generates green tiles on the grid and stores them also in an array
	void SpawnGreenTiles();

	// Creates the blue tile
	void SpawnBlueTile();

	// Checks if a tile has at least one tile that is not red in it's neighborhood (the eight tiles around it)
	bool IsTileReachable(const int32 x, const int32 y) const;
	
	// Given a (x, y) coordinate deduce its associated position on the 3D Space
	FVector Get3DSpaceTileLocation(const int32 x, const int32 y);

	/**
	 * Get the associated tile at a 3D position.
	 *
	 * Retrieves the tile associated with a given 3D position. Returns nullptr if no tile is found.
	 *
	 * @param Position The 3D position in space.
	 * @return The associated tile, or nullptr if none.
	*/
	ATile* GetTileFromPosition(const FVector& InPosition) const;

	// Given a position in 3D space gives the corresponding associated tile coordinates (x, y) 
	// it could give also coordinates outside the grid 
	FVector2D GetCoordinatesFromPosition(const FVector& InPosition) const;

	// Finds the minimum distance in number of tiles to visit in order to reach one of the Tiles still not visited 
	// from a Tile with coordinate of cell x and y
	int32 GetClosestTileDistance(const int32 x, const int32 y, const TArray<TObjectPtr<ATile>>& InTiles) const;

	// Tells if the coordinate (x, y) are the one from the player start position tile
	bool IsNotStartTile(const int32 x, const int32 y) const;

	/**
	 * Called when the gameplay is over, either due to collecting all red or green tiles or
	 * the player falling off the platform.This method takes two parameters :
	 * @param bIsWin Indicates whether the player has won the game(true) or not (false).
	 * @param bForceRestart If true, the game will be restarted immediately, skipping the game over panel.
	*/
	void EndGame(bool bIsWin, bool bForceRestart = false);

	/**
	* Controls the player input mode, allowing you to switch between UI-only and player-only modes.
	*
	* @param bUIOnly When true, sets the input mode to UI Only, enabling interaction with UI widgets.
	*                When false, sets the input mode back to player only, allowing standard gameplay input.
	*
	*/
	void SetPlayerInputModeToUIOnly(bool bUIOnly);

	/* Used to show all the green tiles that have not still been visited */
	void RevealGreenTiles(bool bReveal);

	/** Triggers the behavior of the InTile and ends the game if game ending conditions were found */
	void OnPlayerMoveOnTile(ATile* InTile);

	// Value calculated runtime to spawn tiles around starting platform
	FVector TilesGridCenterPosition;

	// Tile where the player is currently located
	TObjectPtr<ATile> CurrentPlayerTile;

	// Number of green tiles the player has stood on. 
	int32 GreenTilesFound = 0;

	// Number of red tiles the player has stood on. 
	int32 RedTilesFound = 0;

	/**
	 * This collection provides a faster access method for calculating distances from the player. Use it to optimize
	 * gameplay mechanics related to green tiles.
	 */
	TArray<TObjectPtr<ATile>> GreenTilesArray;

	/**
	 * This collection provides a faster access
	 * way for calculating distances from the player. Use it to optimize
	 * gameplay mechanics related to red tiles.
	 */
	TArray<TObjectPtr<ATile>> RedTilesArray;

	// Used to avoid spawning a blue, red, or green tile in the start position
	FVector2D StartTileCoordinates;
};



