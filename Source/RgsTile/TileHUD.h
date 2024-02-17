// Copyright(c) Forge Reply. All Rights Reserved.

#pragma once

#include "GameFramework/HUD.h"
#include "RgsTileGameMode.h"

#include "TileHUD.generated.h"

class UEndScreenWidget;

UCLASS(BlueprintType, Blueprintable)
class RGSTILE_API ATileHUD : public AHUD
{
	GENERATED_BODY()

public:


	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void ShowEndScreen();

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowEndScreenEvent(bool bIsWin, bool bForceRestart);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UEndScreenWidget> EndScreenWidget;

	virtual void BeginPlay() override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


};
