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
	if(TileGrid.Num() > 0  && TileGrid[0].Num() > 0)
		return;

	TileGrid.SetNum(TileGridSize);
	for (int32 i = 0; i < TileGridSize; ++i)
	{
		TileGrid[i].SetNumZeroed(TileGridSize);
	}
	

	const APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (!PlayerController)
		return;
	
	const APawn* PlayerPawn = PlayerController->GetPawn();

	if (!PlayerPawn)
		return;
	
	FVector PlayerLocation = PlayerPawn->GetActorLocation();
	
	FVector StartLocation = PlayerLocation;
	// Calculate spawning starting location
	FRotator SpawnRotation;
	float constexpr SectorSize = 200.f;
	int StartLocationOffset = TileGridSize % 2 == 0 ? 0 : 1;
	StartLocation.X += (TileGridSize - StartLocationOffset)*100.f;
	StartLocation.Y += (TileGridSize - StartLocationOffset)*100.f;

	for (int x = 0; x < TileGridSize; x++)
	{
		for (int y = 0; y < TileGridSize; y++)
		{
			FVector SpawnLocation = FVector(static_cast<float>(x) - static_cast<float>(TileGridSize), 
				static_cast<float>(y) - static_cast<float>(TileGridSize),
				0.f) * SectorSize * 1.f + StartLocation;
			SpawnLocation.Z = -(StartLocation.Z - PlayerPawn->BaseEyeHeight / 2.f) + 16.f;
			ATile* Tile = GetWorld()->SpawnActor<ATile>(NormalTileBP, SpawnLocation, SpawnRotation);
			TileGrid[x][y] = Tile;
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
	SpawnTileGrid();
}

void ARgsTileGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//TODO: implementation
}

