# TileLevel

Mini Game in Unreal where a player has to move aorund a map and interact with the tiles where he/she is walking in

# Requirements

- refactoring

- [Additional] Add tile that turns blu and their behaviour

- [Additional] There should be some BP rules that constrain red tile to be a certain number that doesn't prevent player win (there is no path for the player to win)

- [Additional] Add effect on tile StepOn: red tile shoud tremble, then show smoke and at the end show a red color

- [Additional] Add effect on tile Green StepOn: green tile should tremble and after should show stars and later green color

# Currently working on

- The game should be reset immediately if the player falls off the platform

- Call ATileHUD::ShowEndScreen() if player step on all red/green tiles

- Reset game after about 5 seconds after GameOver() ARgsTileGameMode::ResetGame()

# Done

- Implement all the method from ARgsTileGameMode
- Adding green tiles using some rules
- Handle interactions with ATile classes
- Adding red tiles using similar methods https://www.youtube.com/watch?v=uqylRabVUTo
- Add tile that turns Red
- Add tile that turns Green
- Add tile that turns Silver
- Use C++ inheritance to handle ATiles
- Generate grid reading input from TileGameModeBP
