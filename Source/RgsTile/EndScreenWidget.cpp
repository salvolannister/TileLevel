// Copyright(c) Forge Reply. All Rights Reserved.


#include "EndScreenWidget.h"


#include "Internationalization/Text.h" 
#include "Components/TextBlock.h"

void UEndScreenWidget::OpenEndScreen(bool bIsWin)
{
	PlayAnimationForward(Animation_PanelPopUp);

	if (EndGameText == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT(" End Game block text is null"));
		return;
	}

	if (bIsWin)
	{
		EndGameText->SetText(WinningText);
	}
	else
	{
		EndGameText->SetText(LoosingText);
	}
}

void UEndScreenWidget::CloseEndScreen()
{
	PlayAnimationReverse(Animation_PanelPopUp);
}
