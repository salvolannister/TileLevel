// Copyright(c) Forge Reply. All Rights Reserved.

#include "RgsTileGameMode.h"

#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"

#include "RgsTileCharacter.h"
#include "RgsConfiguration.h"
#include "Tile.h"
#include "TileHUD.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Character.h>



ARgsTileGameMode::ARgsTileGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}


void ARgsTileGameMode::ResetGame()
{
	SetPlayerInputModeToUIOnly(false);
	UGameplayStatics::OpenLevel(this, FName("/Game/Levels/LoadingLevel"));
}

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

int32 ARgsTileGameMode::GetClosestGreenTileDistance()
{

	return GetClosestTileDistance(CurrentPlayerTile->TilePosX, CurrentPlayerTile->TilePosY, GreenTilesArray);
}

int32 ARgsTileGameMode::GetClosestRedTileDistance()
{
	return GetClosestTileDistance(CurrentPlayerTile->TilePosX, CurrentPlayerTile->TilePosY, RedTilesArray);
}

void ARgsTileGameMode::SpawnTileGrid()
{
	if(TileGrid.Num() > 0  && TileGrid[0].Num() > 0)
		return;

	TileGrid.SetNum(TileGridSize);
	for (int32 i = 0; i < TileGridSize; ++i)
	{
		TileGrid[i].SetNumZeroed(TileGridSize);
	}
	

	// Add red tiles
	
	SpawnRedTiles();

	// Add green tiles

	SpawnGreenTiles();
	
	// Add blue tile

	SpawnBlueTile();

	// spawn normal tiles
	for (int32 x = 0; x < TileGridSize; x++)
	{
		for (int32 y = 0; y < TileGridSize; y++)
		{
			if (TileGrid[x][y] == nullptr)
			{
				FVector SpawnLocation = Get3DSpaceTileLocation(x, y);
				ATile* Tile = GetWorld()->SpawnActor<ATile>(NormalTileBP, SpawnLocation, FRotator::ZeroRotator);
				TileGrid[x][y] = Tile;
				Tile->StoreTileGridPosition(x, y);

				Tile->SetRenderText(x, y);
			}
		}
	}

	// Show tiles for debugging
	ShowColoredTiles();
}

void ARgsTileGameMode::ShowColoredTiles()
{
	for (int i = 0; i < TileGridSize; i++)
	{
		for (int ii = 0; ii < TileGridSize; ii++)
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
		int32 x = FMath::RandRange(0, TileGridSize - 1);
		int32 y = FMath::RandRange(0, TileGridSize - 1);


		if (TileGrid[x][y] == nullptr && IsNotStartTile(x, y) && IsGreenTileReachable(x, y))
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
		int32 x = FMath::RandRange(0, TileGridSize - 1);
		int32 y = FMath::RandRange(0, TileGridSize - 1);

		if (!TileGrid[x][y] && IsNotStartTile(x, y) && IsGreenTileReachable(x, y))
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

bool ARgsTileGameMode::IsGreenTileReachable(const int32 x, const int32 y) const
{
	// Looks if there is at least one safe Tile that can lead to the green Tile
    int32 SafeTileNumber = 0;

	for (int32 w = x - 1; w < x + 2; w++)
	{
		if(w >= TileGridSize) break;
		if(w < 0) w++;

		for (int32 h = y - 1; h < h + 2; h++)
		{
			if(h < 0)
			{
				h++;
				continue;
			}
			else if(h >= TileGridSize)
			{
				break;
			}

			if(w == x && h == y) continue;

			if (TileGrid[w][h] == nullptr || TileGrid[w][h]->GetType() != ETileType::Red)
			{
				SafeTileNumber++;
				break;
			}
		}
	}

	return SafeTileNumber > 0;
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
	   int32 x = FMath::RandRange(0, TileGridSize - 1);
	   int32 y = FMath::RandRange(0, TileGridSize - 1);

	  
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

FVector ARgsTileGameMode::Get3DSpaceTileLocation(const int32 x, const int32 y)
{
	FVector SpawnLocation = FVector(static_cast<float>(x) - static_cast<float>(TileGridSize),
		static_cast<float>(y) - static_cast<float>(TileGridSize),
		0.f) * SectorSize * 1.f + TilesGridOffset;
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


	if (x < 0 || x >= TileGridSize || y < 0 || y >= TileGridSize)
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
	FVector TmpPosition = (Position - TilesGridOffset) / SectorSize + TileGridSize;
	return FVector2D(FMath::RoundToInt(TmpPosition.X), FMath::RoundToInt(TmpPosition.Y));
}

void ARgsTileGameMode::SetPlayerInputModeToUIOnly(bool bUIOnly)
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (!PlayerController)
		return;

	FInputModeUIOnly InputModeUI;
	FInputModeGameOnly InputModeGame;

	if (bUIOnly)
	{
		PlayerController->SetIgnoreMoveInput(true);
		PlayerController->SetInputMode(InputModeUI);

	}
	else
	{
		PlayerController->SetIgnoreMoveInput(false);
		PlayerController->SetInputMode(InputModeGame);
	}
}


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

void ARgsTileGameMode::BeginPlay()
{
	Super::BeginPlay();

	const APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (!PlayerController)
		return;

	const APawn* PlayerPawn = PlayerController->GetPawn();

	if (!PlayerPawn)
		return;

	TilesGridOffset = PlayerPawn->GetActorLocation();
	int TileGridSizeOffset = TileGridSize % 2 == 0 ? 0 : 1;
	TilesGridOffset.X += (TileGridSize + TileGridSizeOffset) * 100.f;
	TilesGridOffset.Y += (TileGridSize + TileGridSizeOffset) * 100.f;
	TilesGridOffset.Z = -(TilesGridOffset.Z - PlayerPawn->BaseEyeHeight / 2.f) + 16.f;
	
	StartTileCoordinates = GetCoordinatesFromPosition(PlayerPawn->GetActorLocation());

	SpawnTileGrid();
	
	CurrentPlayerTile = GetTileFromPosition(PlayerPawn->GetActorLocation());

	GreenTilesFound = RedTilesFound = 0;

}

void ARgsTileGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ATile* T = GetTileFromPosition(GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation());
	if (T && CurrentPlayerTile && T != CurrentPlayerTile)
	{
		CurrentPlayerTile->StepOff();
	
		// Player was on a blue Tile and now is stepping in a new Tile T
		if (CurrentPlayerTile->IsA(BlueTileBP))
		{
			RevealGreenTiles(false);
		}

		CurrentPlayerTile = T;

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

	// Player out of grid
	if (T == nullptr)
	{
		EndGame(false, true);
	}
	
}

int32 ARgsTileGameMode::GetClosestTileDistance(const int32 x, const int32 y, const TArray<TObjectPtr<ATile>>& InTiles) const
{
	/* To determine the minimum, we consider the maximum distance module between the distances in the x and y directions.
	 * This ensures that an element on the diagonal e.g. (x + 1, y + 1) of the current coordinates(x, y) is equidistant 
	 * from one on the sides e.g. (x + 1, y).
	 */

	if (InTiles.Num() == 0)
		return -1;

	int32 MinDistanceFound = TileGridSize;

	for (const ATile* TilePtr : InTiles)
	{
		if (TilePtr && !TilePtr->HasBeenVisited())
		{
			int32 DistanceOnX = FMath::Abs(x - TilePtr->TilePosX);
			int32 DistanceOnY = FMath::Abs( y - TilePtr->TilePosY);

			int32 NumTileToReachPos = FMath::Max(DistanceOnX, DistanceOnY);

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

bool ARgsTileGameMode::IsNotStartTile(const int32 x, const int32 y) const
{
	
	if (x != StartTileCoordinates.X || y != StartTileCoordinates.Y)
		return true;
	else
		return false;

}

void ARgsTileGameMode::EndGame(bool bIsWin, bool bForceRestart)
{
	
	SetPlayerInputModeToUIOnly(true);

	if (OnEndGameDelegate.IsBound())
	{
		OnEndGameDelegate.Broadcast(bIsWin, bForceRestart);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No subscribers to OnEndGameDelegate"));
	}
	
	if (bForceRestart)
	{
		ResetGame();
	}
	
}

