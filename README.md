# TileLevel

Mini Game in Unreal where a player has to move aorund a map and interact with the tiles where he/she is walking in

# Requirements

- stop input when game is over

- if player jumping tile shouldn't become colored

- initialization can be done in header files too, but don't split it both in header and cpp files

- [Additional] Add tile that turns blu and its behaviour

- [Additional] There should be some BP rules that constrain red tiles to be a certain number that doesn't prevent player win (there is no path for the player to win)

- [Additional] Add effect on tile StepOn: red tile shoud tremble, then show smoke and at the end show a red color

- [Additional] Add effect on tile Green StepOn: green tile should tremble and after should show stars and later green color

- [Additional] Add sounds 

- [Additional] Try to understand that calling the interface in this way is a bad idea

# Currently working on

- improve certain methods that now gets called continously

# Doubts

- ask someone if the implementation of the HUD half in the BP and half in cpp is a red flag or is ok:
  I did this because it was implemented already in the blueprint but I wanted to add an animation and the faster way I knew to do it
  was using cpp 
--> ANSWER: just tell them that since it was already half implemeted and was a test you left it there but for production you would have move everything inside cpp

- Reloading the scene was already in the code base, but it kinda feels very bad for optimization. Should I remove it?

- If in ATile the basic implementation is absent should I remove it from the cpp file?

- Should I remove this log or set it between #WITH_DEBUG?
	if (EndGameText.IsNull())
	{
		UE_LOG(LogTemp, Error, TEXT(" End Game block text is null"));
		return;
	}

# Resources

# Done

- The game should be reset immediately if the player falls off the platform
- Call ATileHUD::ShowEndScreen() if player step on all red/green tiles
- Reset game after about 5 seconds after GameOver() ARgsTileGameMode::ResetGame()
- Implement all the method from ARgsTileGameMode
- Adding green tiles using some rules
- Handle interactions with ATile classes
- Adding red tiles using similar methods https://www.youtube.com/watch?v=uqylRabVUTo
- Add tile that turns Red
- Add tile that turns Green
- Add tile that turns Silver
- Use C++ inheritance to handle ATiles
- Generate grid reading input from TileGameModeBP
- Add comments to methods and check naming conventions
- add In to varibale that are not modified and passed by refererence
- add const to every parameter that is not intended to be modified
- add comment about return expected type where you pass null