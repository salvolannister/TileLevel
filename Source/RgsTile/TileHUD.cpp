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

	auto CloseEndScreenDelayed = [this]() {
		if (EndScreenWidget)
		{
			EndScreenWidget->CloseEndScreen();
		}
	};

	
	GetWorldTimerManager().SetTimer(CloseEndScreenPanelTimerHandle, CloseEndScreenDelayed, 4.f, false);
	
	
}

void ATileHUD::BeginPlay()
{
	Super::BeginPlay();

	if (const UWorld* World = GetWorld())
	{
		if (AGameModeBase* Game = World->GetAuthGameMode())
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
	Super::EndPlay(EndPlayReason);

	if (const UWorld*  World = GetWorld())
	{
		if (AGameModeBase* Game = World->GetAuthGameMode())
		{
			ARgsTileGameMode* TileGameMode = Cast<ARgsTileGameMode>(Game);;
			if (TileGameMode)
			{
				TileGameMode->OnEndGameDelegate.RemoveDynamic(this, &ATileHUD::ShowEndScreenEvent);
			}
		}
		// If world doesn't exist at this point neither will the timer
		GetWorldTimerManager().ClearTimer(CloseEndScreenPanelTimerHandle);
	}




}

