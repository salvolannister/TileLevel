// Copyright(c) Forge Reply. All Rights Reserved.

#pragma once

#include "GameFramework/HUD.h"
#include "RgsTileGameMode.h"

#include "TileHUD.generated.h"

class UEndScreenWidget;
/**
* Handles the info about red/green tiles and 
* adds the end screen panel to viewport inside the blueprint.
*/
UCLASS(BlueprintType, Blueprintable)
class RGSTILE_API ATileHUD : public AHUD
{
	GENERATED_BODY()

public:

	// Adds Endscreen to viewport, implemented in TileGameHud blueprint
	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void ShowEndScreen();

	// Method used to subscribe to the EndGame event in RgsTileGameMode and open the EndScreenWidget
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowEndScreenEvent(bool bIsWin, bool bForceRestart);
	
	// EndScreenWidget class used to spawn the End Screen panel in viewport used by ShowEndScreen
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UEndScreenWidget> EndScreenWidget;

	virtual void BeginPlay() override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	/*TimerHandle to close the EndScreen panel*/
	FTimerHandle CloseEndScreenPanelTimerHandle;
};
