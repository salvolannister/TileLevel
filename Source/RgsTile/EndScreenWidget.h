// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndScreenWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, meta = (DisableNativeTick))
class RGSTILE_API UEndScreenWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidget))
	TObjectPtr<UTextBlock> GreenCollectedTilesText;
	
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidget))
	TObjectPtr<UTextBlock> RedCollectedTilesText;
	
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidget))
	TObjectPtr<UTextBlock> EndGameText;
	
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Animation_PanelPopUp;

	UPROPERTY(BlueprintCallable)
	void OpenEndScreen(int32 GreenTilesCollected, int32 RedTilesCollected, bool bIsWin);
	
	UPROPERTY(BlueprintCallable)
	void CloseEndScreen();
};
