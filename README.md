# TileLevel

Mini Game in Unreal where a player has to move aorund a map and interact with the tiles where he/she is walking in

# Requirements

- [ Additional ] Add tile that turns blu and its behaviour

- [ Additional ] There should be some BP rules that constrain red tiles to be a certain number that doesn't prevent player win (there is no path for the player to win)

- [ Additional ] Add effect on tile StepOn: red tile shoud tremble, then show smoke and at the end show a red color

- [ Additional ] Add effect on tile Green StepOn: green tile should tremble and after should show stars and later green color

- [ Additional ] Add sounds 

- [ Additional ] Try to understand that calling the interface in this way is a bad idea

# Currently working on

- Add comments to methods, check naming conventions, improve certain methods that now gets called continously

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
