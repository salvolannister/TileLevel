// Copyright(c) Forge Reply. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <Blueprint/UserWidget.h>

#include "EndScreenWidget.generated.h"

class UTextBlock;
class UText;

/**
 * Manages open and close for the EndScreen panel blueprint
 * with animations and title updates.
 */
UCLASS(BlueprintType, Blueprintable, meta = (DisableNativeTick))
class RGSTILE_API UEndScreenWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	/** Text to display when the player wins */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText WinningText;

	/** Text to display when the player loses */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText LoosingText;
	
	/** Pointer to correspondent variable in the blueprint */
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidget))
	TObjectPtr<UTextBlock> EndGameText;
	
	/** Animation defined in the blueprint widget where the panel scales 0 to full screen size */
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Animation_PanelPopUp;


	/**
	* Set the visibility to Not-Hit-Testable, plays the AnimationPopUp 
	* and updates the EndGameText in the panel based on the game outcome.
	* @param bIsWin True if the game is won, false if it's a loss.
	*/
	void OpenEndScreen(bool bIsWin);
	
	/** Plays the AnimationPopUp in reverse state and set the visibility to collapsed */
	void CloseEndScreen();


};
