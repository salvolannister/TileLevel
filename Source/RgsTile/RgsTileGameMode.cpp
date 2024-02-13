// Copyright(c) Forge Reply. All Rights Reserved.

#include "RgsTileGameMode.h"
#include "RgsTileCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Tile.h"
#include "Kismet/GameplayStatics.h"
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
	// avoids possible multiplication overflow
	long int GridDim = (long)TileGridSize * TileGridSize;

	if (TileGrid.size() < GridDim)
	{
		TileGrid.resize(TileGridSize, std::vector < TObjectPtr<ATile>>(TileGridSize));
	}

	// Get controller location
	FVector StartLocation;
	// Calculate spawning starting location
	FRotator SpawnRotation;

	for (int x = 0; x < TileGridSize; x++)
	{
		for (int y = 0; y < TileGridSize; y++)
		{

			FVector SpawnLocation = StartLocation;
			ATile* Tile = GetWorld()->SpawnActor<ATile>(NormalTileBP, SpawnLocation, SpawnRotation);
			//TileGrid[x][y] = 
		}
	}
}

void ARgsTileGameMode::ShowColoredTiles()
{

}

void ARgsTileGameMode::BeginPlay()
{
	Super::BeginPlay();

	//TODO: implementation
}

void ARgsTileGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//TODO: implementation
}

