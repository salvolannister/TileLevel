#include "RgsTileGameMode.h"

#include <UObject/ConstructorHelpers.h>
#include <Kismet/GameplayStatics.h>
#include <Math/UnrealMathUtility.h>
#include <GameFramework/Character.h>

#include "RgsTileCharacter.h"
#include "RgsConfiguration.h"
#include "Tile.h"
#include "TileHUD.h"



ARgsTileGameMode::ARgsTileGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void ARgsTileGameMode::BeginPlay()
{
	Super::BeginPlay();

	const APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (!PlayerController)
		return;

	RgsPawn = PlayerController->GetPawn();

	if (!RgsPawn.Get())
		return;

	const int32 TileGridSizeOffset = EdgeGridSize % 2 == 0 ? 0 : 1;
	const float HalfSectorSize = SectorSize/2.f;
	TilesGridCenterPosition = RgsPawn->GetActorLocation();
	TilesGridCenterPosition.X += (EdgeGridSize + TileGridSizeOffset) * HalfSectorSize;
	TilesGridCenterPosition.Y += (EdgeGridSize + TileGridSizeOffset) * HalfSectorSize;
	TilesGridCenterPosition.Z = -(TilesGridCenterPosition.Z - RgsPawn->GetDefaultHalfHeight());
	
	StartTileCoordinates = GetCoordinatesFromPosition(RgsPawn->GetActorLocation());
	SpawnTileGrid();
	CurrentPlayerTile = GetTileFromPosition(RgsPawn->GetActorLocation());
	GreenTilesFound = RedTilesFound = 0;

}

void ARgsTileGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!RgsPawn.Get())
	{
		UE_LOG(LogTemp, Error, TEXT("Pawn is null, this shouldn't happen"));
		return;
	}

	// Check to avoid making calculation every frame
	if (!RgsPawn->GetVelocity().IsZero())
	{
		ATile* T = GetTileFromPosition(RgsPawn->GetActorLocation());
		OnPlayerMoveOnTile(T);
	}
	

	
}

#pragma region Gameplay

void ARgsTileGameMode::OnPlayerMoveOnTile(ATile* InTile)
{
	// If the Tile is null the player is out of the grid
	if (InTile == nullptr)
	{
		EndGame(false, true);
	}

	if (CurrentPlayerTile && InTile != CurrentPlayerTile)
	{
		CurrentPlayerTile->StepOff();

		// To avoid holding a reference of the GameMode in the blue tile class we call what
		// the tile should do for the step off here
		if (CurrentPlayerTile->IsA(BlueTileBP))
		{
			RevealGreenTiles(false);
		}

		CurrentPlayerTile = InTile;

		if (CurrentPlayerTile->IsA(RedTileBP) && !CurrentPlayerTile->HasBeenVisited())
		{
			RedTilesFound++;
			RedTilesFound = FMath::Clamp(RedTilesFound, 0, RedTilesToSpawn);
			if (RedTilesFound == RedTilesToSpawn)
			{
				EndGame(false);
			}
		}
		else if (CurrentPlayerTile->IsA(GreenTileBP) && !CurrentPlayerTile->HasBeenVisited())
		{
			GreenTilesFound++;
			GreenTilesFound = FMath::Clamp(GreenTilesFound, 0, GreenTilesToSpawn);

			if (GreenTilesFound == GreenTilesToSpawn)
			{
				EndGame(true);
			}
		}
		else if (CurrentPlayerTile->IsA(BlueTileBP))
		{
			RevealGreenTiles(true);
		}

		CurrentPlayerTile->StepOn();
	}

}

void ARgsTileGameMode::ResetGame()
{
	SetPlayerInputModeToUIOnly(false);
	UGameplayStatics::OpenLevel(this, FName("/Game/Levels/LoadingLevel"));
}

void ARgsTileGameMode::EndGame(bool bIsWin, bool bForceRestart)
{
	if (bForceRestart)
	{
		ResetGame();
	}
	
	SetPlayerInputModeToUIOnly(true);

	if (OnEndGameDelegate.IsBound())
	{
		OnEndGameDelegate.Broadcast(bIsWin, bForceRestart);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No subscribers to OnEndGameDelegate"));
	}
}

void ARgsTileGameMode::SetPlayerInputModeToUIOnly(bool bUIOnly)
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (!PlayerController)
		return;

	if (bUIOnly)
	{
		FInputModeUIOnly InputModeUI;
		PlayerController->SetIgnoreMoveInput(true);
		PlayerController->SetInputMode(InputModeUI);

	}
	else
	{
		FInputModeGameOnly InputModeGame;
		PlayerController->SetIgnoreMoveInput(false);
		PlayerController->SetInputMode(InputModeGame);
	}
}

#pragma endregion

int32 ARgsTileGameMode::GetTotalGreenTiles()
{
	return GreenTilesToSpawn;
}

int32 ARgsTileGameMode::GetGreenTilesFound()
{
	return GreenTilesFound;
}

int32 ARgsTileGameMode::GetTotalRedTiles()
{
	return RedTilesToSpawn;
}

int32 ARgsTileGameMode::GetRedTilesFound()
{
	return RedTilesFound;
}

#pragma region Tiles Distances
int32 ARgsTileGameMode::GetClosestGreenTileDistance()
{

	return GetClosestTileDistance(CurrentPlayerTile->TilePosX, CurrentPlayerTile->TilePosY, GreenTilesArray);
}

int32 ARgsTileGameMode::GetClosestRedTileDistance()
{
	return GetClosestTileDistance(CurrentPlayerTile->TilePosX, CurrentPlayerTile->TilePosY, RedTilesArray);
}

int32 ARgsTileGameMode::GetClosestTileDistance(const int32 X, const int32 Y, const TArray<TObjectPtr<ATile>>& InTiles) const
{
	/* To determine the minimum, we consider the sum of the distance module between the distances in the x and y directions.
	 * An element on the diagonal e.g. (x + 1, y + 1) of the current coordinates (x, y) won't be reachable through one step of our current pawn,
	 * so it is considered at distance two instead of one
	 */

	if (InTiles.Num() == 0)
		return -1;

	int32 MinDistanceFound = EdgeGridSize;

	for (const ATile* TilePtr : InTiles)
	{
		if (TilePtr && !TilePtr->HasBeenVisited())
		{
			int32 DistanceOnX = FMath::Abs(X - TilePtr->TilePosX);
			int32 DistanceOnY = FMath::Abs( Y - TilePtr->TilePosY);

			int32 NumTileToReachPos = DistanceOnX + DistanceOnY;

			if (MinDistanceFound > NumTileToReachPos)
			{
				MinDistanceFound = NumTileToReachPos;
				if (MinDistanceFound == 1)
				{
					// stop looking since the minimum possible distance was found
					break;
				}
			}
		}
	}

	return MinDistanceFound;
}

#pragma endregion

#pragma region Tiles Spawning
void ARgsTileGameMode::SpawnTileGrid()
{
	if(TileGrid.Num() > 0  && TileGrid[0].Num() > 0)
		return;

	TileGrid.SetNum(EdgeGridSize);
	for (int32 i = 0; i < EdgeGridSize; ++i)
	{
		TileGrid[i].SetNumZeroed(EdgeGridSize);
	}
		
	SpawnRedTiles();

	SpawnGreenTiles();
	
	SpawnBlueTile();

	// Fills the empty spots in the grid with normal tiles starting from the top right tile
	// going to the bottom left one
	for (int32 x = 0; x < EdgeGridSize; x++)
	{
		for (int32 y = 0; y < EdgeGridSize; y++)
		{
			if (TileGrid[x][y] == nullptr)
			{
				FVector SpawnLocation = Get3DSpaceTileLocation(x, y);
				ATile* Tile = GetWorld()->SpawnActor<ATile>(NormalTileBP, SpawnLocation, FRotator::ZeroRotator);
				TileGrid[x][y] = Tile;
				Tile->StoreTileGridPosition(x, y);
#if WITH_DEBUG
				Tile->SetRenderText(x, y);
#endif
			}
		}
	}
#if WITH_DEBUG && SHOW_TILES_COLOR
	ShowColoredTiles();
#endif
}

void ARgsTileGameMode::ShowColoredTiles()
{
	for (int i = 0; i < EdgeGridSize; i++)
	{
		for (int ii = 0; ii < EdgeGridSize; ii++)
		{
			TileGrid[i][ii]->ShowTileColor(true);
		}
	}
}

void ARgsTileGameMode::SpawnGreenTiles()
{					   
	if (TileGrid.Num() == 0 || TileGrid[0].Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Tile grid is not initialized!"));
		return;
	}

	for (int32 i = GreenTilesToSpawn; i > 0; i--)
	{
		int32 x = FMath::RandRange(0, EdgeGridSize - 1);
		int32 y = FMath::RandRange(0, EdgeGridSize - 1);


		if (TileGrid[x][y] == nullptr && IsNotStartTile(x, y) && IsTileReachable(x, y))
		{
		
			FVector SpawnLocation = Get3DSpaceTileLocation(x, y);
			ATile* Tile = GetWorld()->SpawnActor<ATile>(GreenTileBP, SpawnLocation, FRotator::ZeroRotator);
			TileGrid[x][y] = Tile;
			GreenTilesArray.Add(Tile);
			Tile->StoreTileGridPosition(x, y);

#if WITH_DEBUG
			Tile->SetRenderText(x, y);
#endif

		}
		else
		{
			i++;
		}
	}
}					   

void ARgsTileGameMode::SpawnBlueTile()
{
	if (TileGrid.Num() == 0 || TileGrid[0].Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Tile grid is not initialized!"));
		return;
	}

	if (!BlueTileBP)
	{
		UE_LOG(LogTemp, Error, TEXT("BlueTileBP is not set in blueprint"));
		return;
	}


	int32 NAttempts = 10;
	bool bBlueTileSpawned = false;
	while (NAttempts > 0 && !bBlueTileSpawned)
	{
		int32 x = FMath::RandRange(0, EdgeGridSize - 1);
		int32 y = FMath::RandRange(0, EdgeGridSize - 1);

		if (!TileGrid[x][y] && IsNotStartTile(x, y) && IsTileReachable(x, y))
		{
		
				FVector SpawnLocation = Get3DSpaceTileLocation(x, y);
				ATile* Tile = GetWorld()->SpawnActor<ATile>(BlueTileBP, SpawnLocation, FRotator::ZeroRotator);
				TileGrid[x][y] = Tile;
				Tile->StoreTileGridPosition(x, y);
				bBlueTileSpawned = true;

#if WITH_DEBUG
				Tile->SetRenderText(x, y);
#endif
		}
		else
		{
			NAttempts --;
		}

	}
}

void ARgsTileGameMode::SpawnRedTiles()
{
	if (TileGrid.Num() == 0 || TileGrid[0].Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Tile grid is not initialized!"));
		return;
	}

	for (int32 i = RedTilesToSpawn; i > 0; i--)
	{
	   int32 x = FMath::RandRange(0, EdgeGridSize - 1);
	   int32 y = FMath::RandRange(0, EdgeGridSize - 1);

	  
	   if (TileGrid[x][y] == nullptr && IsNotStartTile(x, y))
	   {
			FVector SpawnLocation = Get3DSpaceTileLocation(x, y);
			ATile* Tile = GetWorld()->SpawnActor<ATile>(RedTileBP, SpawnLocation, FRotator::ZeroRotator);
			TileGrid[x][y] = Tile;
			RedTilesArray.Add(Tile);
			
			Tile->StoreTileGridPosition(x, y);
#if WITH_DEBUG
			Tile->SetRenderText(x, y);
#endif
	   }
	   else 
	   {
		   i++;
	   }
	}
}

#pragma endregion 

#pragma region Tiles Coordinates
bool ARgsTileGameMode::IsTileReachable(const int32 X, const int32 Y) const
{
	// Looks if there is at least one tile that can lead to the green Tile without pressing a red tile
    int32 SafeTileNumber = 0;

	auto IsTileSafeToCross = [&](int32 x, int32 y) -> bool
	{
		if (TileGrid.IsValidIndex(x) && TileGrid[x].IsValidIndex(y))
		{
			return (TileGrid[x][y] == nullptr || TileGrid[x][y].IsA(RedTileBP));
		}

		return false;
	};

	if(IsTileSafeToCross(X + 1, Y) || IsTileSafeToCross(X - 1, Y) ||
		IsTileSafeToCross(X, Y + 1) || IsTileSafeToCross(X, Y -1))
	{
		SafeTileNumber++;
	}




	return SafeTileNumber > 0;
}

FVector ARgsTileGameMode::Get3DSpaceTileLocation(const int32 X, const int32 Y) const 
{
	FVector SpawnLocation = FVector(static_cast<float>(X) - static_cast<float>(EdgeGridSize),
		static_cast<float>(Y) - static_cast<float>(EdgeGridSize),
		0.f) * SectorSize * 1.f + TilesGridCenterPosition;
	return SpawnLocation;
}

ATile* ARgsTileGameMode::GetTileFromPosition(const FVector& Position) const
{
	FVector2D TmpPosition = GetCoordinatesFromPosition(Position);
	int32 x = FMath::RoundToInt(TmpPosition.X);
	int32 y = FMath::RoundToInt(TmpPosition.Y);

#if WITH_DEBUG
	FString DebugMessage;
	if (GEngine)
	{
		DebugMessage = FString::Printf(TEXT("PlayerPos is x %.2f, y %.2f"), Position.X, Position.Y);
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, DebugMessage, false);
		DebugMessage = FString::Printf(TEXT("TmpPosition: x %.2f, y %.2f "), TmpPosition.X, TmpPosition.Y);
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, DebugMessage, false);

	}
#endif


	if (x < 0 || x >= EdgeGridSize || y < 0 || y >= EdgeGridSize)
	{

#if WITH_DEBUG
		DebugMessage = FString::Printf(TEXT("Found x %d, y %d which is not in the screen"), x, y);

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, DebugMessage, false);
		}
#endif
		return nullptr;
	}
#if WITH_DEBUG
	else
	{

		DebugMessage = FString::Printf(TEXT("Player on grid x %d, y %d "), x, y);

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, DebugMessage, false);
		}

	}
#endif

	return TileGrid[x][y];

}

FVector2D ARgsTileGameMode::GetCoordinatesFromPosition(const FVector& Position) const
{
	FVector TmpPosition = (Position - TilesGridCenterPosition) / SectorSize + EdgeGridSize;
	return FVector2D(FMath::RoundToInt(TmpPosition.X), FMath::RoundToInt(TmpPosition.Y));
}

#pragma endregion


void ARgsTileGameMode::RevealGreenTiles(bool bReveal)
{
	for(ATile* Tile : GreenTilesArray)
	{
		if (!Tile->HasBeenVisited())
		{
			Tile->ShowTileColor(bReveal);
		}
	}
}

bool ARgsTileGameMode::IsNotStartTile(const int32 X, const int32 Y) const
{
	
	if (X != StartTileCoordinates.X || Y != StartTileCoordinates.Y)
		return true;
	else
		return false;

}


