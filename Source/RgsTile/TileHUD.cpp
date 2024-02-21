#include "TileHUD.h"

#include "EndScreenWidget.h"
#include "RgsTileGameMode.h"


void ATileHUD::ShowEndScreenEvent(bool bIsWin, bool bForceRestart)
{
	if (bForceRestart)
	{
		return;
	}

	ShowEndScreen();

	if (EndScreenWidget)
	{
		EndScreenWidget->OpenEndScreen(bIsWin);
	}

	GetWorldTimerManager().SetTimer(CloseEndScreenPanelTimerHandle, [this]() { EndScreenWidget->CloseEndScreen(); }, 4.f, false);

	
	
}

void ATileHUD::BeginPlay()
{
	Super::BeginPlay();

	UWorld* const World = GetWorld();
	if (World)
	{
		AGameModeBase* const Game = World->GetAuthGameMode();
		if (Game)
		{
			ARgsTileGameMode* TileGameMode = Cast<ARgsTileGameMode>(Game);;
			if (TileGameMode)
			{
				TileGameMode->OnEndGameDelegate.AddUniqueDynamic(this, &ATileHUD::ShowEndScreenEvent);
			}
		}
	}
}

void ATileHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UWorld* const World = GetWorld();
	if (World)
	{
		AGameModeBase* const Game = World->GetAuthGameMode();
		if (Game)
		{
			ARgsTileGameMode* TileGameMode = Cast<ARgsTileGameMode>(Game);;
			if (TileGameMode)
			{
				TileGameMode->OnEndGameDelegate.RemoveDynamic(this, &ATileHUD::ShowEndScreenEvent);
			}
		}
	}

	Super::EndPlay(EndPlayReason);
}

