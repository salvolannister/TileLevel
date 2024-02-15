// Copyright(c) Forge Reply. All Rights Reserved.

#include "RgsTileGameMode.h"

#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"

#include "RgsTileCharacter.h"
#include "Tile.h"
#include "TileHUD.h"

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
	//TODO: implementation
	return -1;
}

int32 ARgsTileGameMode::GetClosestRedTileDistance()
{
	//TODO: implementation
	return -1;
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
	
	// Add blue tiles

	// spawn normal tiles
	for (int32 x = 0; x < TileGridSize; x++)
	{
		for (int32 y = 0; y < TileGridSize; y++)
		{
			if (TileGrid[x][y] == nullptr)
			{
				FVector SpawnLocation = Get3DSpaceTileLocation(x, y);
				ATile* Tile = GetWorld()->SpawnActor<ATile>(NormalTileBP, SpawnLocation, FRotator::ZeroRotator);
				Tile->SetRenderText(x, y);
				TileGrid[x][y] = Tile;

			}
		}
	}

	// Show tiles for debugging
	//ShowColoredTiles();
}


void ARgsTileGameMode::ShowColoredTiles()
{
	for (int i = 0; i < TileGridSize; i++)
	{
		for (int ii = 0; ii < TileGridSize; ii++)
		{
			TileGrid[i][ii]->ShowTileEffect(true);
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

	if (GreenTilesArray.Num() == 0)
	{
		GreenTilesArray.SetNum(GreenTilesToSpawn);
	}


	for (int32 i = GreenTilesToSpawn; i > 0; i--)
	{
		int32 x = FMath::RandRange(0, TileGridSize - 1);
		int32 y = FMath::RandRange(0, TileGridSize - 1);


		if (TileGrid[x][y] == nullptr && IsGreenTileReachable(x, y))
		{
		
			FVector SpawnLocation = Get3DSpaceTileLocation(x, y);
			ATile* Tile = GetWorld()->SpawnActor<ATile>(GreenTileBP, SpawnLocation, FRotator::ZeroRotator);
			TileGrid[x][y] = Tile;
			GreenTilesArray.Add(Tile);

			Tile->SetRenderText(x, y);
		}
		else
		{
			i++;
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

	if (RedTilesArray.Num() == 0)
	{
		RedTilesArray.SetNum(RedTilesToSpawn);
	}

	for (int32 i = RedTilesToSpawn; i > 0; i--)
	{
	   int32 x = FMath::RandRange(0, TileGridSize - 1);
	   int32 y = FMath::RandRange(0, TileGridSize - 1);

	  
	   if (TileGrid[x][y] == nullptr)
	   {
			FVector SpawnLocation = Get3DSpaceTileLocation(x, y);
			ATile* Tile = GetWorld()->SpawnActor<ATile>(RedTileBP, SpawnLocation, FRotator::ZeroRotator);
			TileGrid[x][y] = Tile;
			Tile->SetRenderText(x, y);
			RedTilesArray.Add(Tile);
	   }
	   else
	   {
		   i++;
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
	TilesGridOffset.X += (TileGridSize - TileGridSizeOffset) * 100.f;
	TilesGridOffset.Y += (TileGridSize - TileGridSizeOffset) * 100.f;
	TilesGridOffset.Z = -(TilesGridOffset.Z - PlayerPawn->BaseEyeHeight / 2.f) + 16.f;
	
	SpawnTileGrid();
	
	CurrentPlayerTile = GetTileFromPosition(PlayerPawn->GetActorLocation());
	//TODO: implementation

	GreenTilesFound = RedTilesFound = 0;

}

void ARgsTileGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Has Player Moved ? 
		// Check if it is in a tile
		// if it is turn on the tile
	ATile* T = GetTileFromPosition(GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation());
	if (T && CurrentPlayerTile && T != CurrentPlayerTile)
	{
		CurrentPlayerTile->StepOff();
		CurrentPlayerTile = T;

		if (CurrentPlayerTile->IsA(RedTileBP) && !CurrentPlayerTile->HasBeenVisited())
		{
			RedTilesFound++;
			RedTilesFound = FMath::Clamp(RedTilesFound, 0, RedTilesToSpawn);

		}
		else if (CurrentPlayerTile->IsA(GreenTileBP) && !CurrentPlayerTile->HasBeenVisited())
		{
			GreenTilesFound++;
			GreenTilesFound = FMath::Clamp(GreenTilesFound, 0, GreenTilesToSpawn);
		}

		CurrentPlayerTile->StepOn();
	}
	
}


FVector ARgsTileGameMode::Get3DSpaceTileLocation(const int32 x, const int32 y)
{
	FVector SpawnLocation = FVector(static_cast<float>(x) - static_cast<float>(TileGridSize),
		static_cast<float>(y) - static_cast<float>(TileGridSize),
		0.f) * SectorSize * 1.f + TilesGridOffset;
	return SpawnLocation;
}


ATile* ARgsTileGameMode::GetTileFromPosition(FVector Position) const
{
	FVector TmpPosition = (Position - TilesGridOffset) / SectorSize + TileGridSize;
	int32 x = static_cast<int32>(roundf(TmpPosition.X));
	int32 y = static_cast<int32>(roundf(TmpPosition.Y));

	FString DebugMessage;
	if (GEngine)
	{
		DebugMessage = FString::Printf(TEXT("PlayerPos is x %.2f, y %.2f"), Position.X, Position.Y);
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, DebugMessage, false);
		DebugMessage = FString::Printf(TEXT("TmpPosition: x %.2f, y %.2f "), TmpPosition.X, TmpPosition.Y);
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, DebugMessage, false);

	}

	if (x < 0 || x >= TileGridSize || y < 0 || y >= TileGridSize)
	{
		
		DebugMessage = FString::Printf(TEXT("Found x %d, y %d which is not in the screen"), x, y);

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, DebugMessage, false);
		}
		return nullptr;
	}
	else
	{
		DebugMessage = FString::Printf(TEXT("Player on grid x %d, y %d "), x, y);

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, DebugMessage, false);
		}

	}

	return TileGrid[x][y];

}


