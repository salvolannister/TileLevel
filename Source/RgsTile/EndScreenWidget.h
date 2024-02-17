// Copyright(c) Forge Reply. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "EndScreenWidget.generated.h"

class UTextBlock;
class UText;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, meta = (DisableNativeTick))
class RGSTILE_API UEndScreenWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText WinningText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText LoosingText;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidget))
	TObjectPtr<UTextBlock> GreenResultText;
	
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidget))
	TObjectPtr<UTextBlock> RedResultText;
	
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidget))
	TObjectPtr<UTextBlock> EndGameText;
	
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Animation_PanelPopUp;

	//UPROPERTY(BlueprintCallable)
	void OpenEndScreen(bool bIsWin);
	
	//UPROPERTY(BlueprintCallable)
	void CloseEndScreen();
};
