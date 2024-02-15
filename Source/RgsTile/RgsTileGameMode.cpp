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
	//TODO: implementation
	return -1;
}

int32 ARgsTileGameMode::GetGreenTilesFound()
{
	//TODO: implementation
	return -1;
}

int32 ARgsTileGameMode::GetTotalRedTiles()
{
	//TODO: implementation
	return -1;
}

int32 ARgsTileGameMode::GetRedTilesFound()
{
	//TODO: implementation
	return -1;
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
		
	for (int32 x = 0; x < TileGridSize; x++)
	{
		for (int32 y = 0; y < TileGridSize; y++)
		{
			FVector SpawnLocation = GetTileLocation(x, y);
			ATile* Tile = GetWorld()->SpawnActor<ATile>(NormalTileBP, SpawnLocation, FRotator::ZeroRotator);
			Tile->SetRenderText(x, y);
			TileGrid[x][y] = Tile;
		}
	}

	// Add red tiles
	
	SpawnRedTiles();

	// Add green tiles

	SpawnGreenTiles();
	
	// Add blue tiles

	// Show tiles for debugging
	ShowColoredTiles();
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
					   
}					   
					   
void ARgsTileGameMode::SpawnRedTiles()
{
	if (TileGrid.Num() == 0 || TileGrid[0].Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Tile grid is not initialized!"));
		return;
	}

	const APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (!PlayerController)
		return;

	const APawn* PlayerPawn = PlayerController->GetPawn();

	if (!PlayerPawn)
		return;

	for (int32 i = RedTilesToSpawn; i > 0; i--)
	{
	   int32 x = FMath::RandRange(0, TileGridSize - 1);
	   int32 y = FMath::RandRange(0, TileGridSize - 1);

	  
	   if (TileGrid[x][y]->GetType() == ETileType::Normal)
	   {
			ATile* TmpTile = TileGrid[x][y];
			TmpTile->Destroy();
			FVector SpawnLocation = GetTileLocation(x, y);
			ATile* Tile = GetWorld()->SpawnActor<ATile>(RedTileBP, SpawnLocation, FRotator::ZeroRotator);
			TileGrid[x][y] = Tile;
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
	
	//TODO: implementation
	SpawnTileGrid();
}

void ARgsTileGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//TODO: implementation
}


FVector ARgsTileGameMode::GetTileLocation(const int32 x, const int32 y)
{
	FVector SpawnLocation = FVector(static_cast<float>(x) - static_cast<float>(TileGridSize),
		static_cast<float>(y) - static_cast<float>(TileGridSize),
		0.f) * SectorSize * 1.f + TilesGridOffset;
	return SpawnLocation;
}
