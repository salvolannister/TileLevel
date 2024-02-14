# TileLevel
Mini Game in Unreal where a player has to move aorund a map and interact with the tiles where he/she is walking in

# Requirements

- call ATileHUD::ShowEndScreen() if player step on all red/green tiles
- reset game after about 5 seconds ARgsTileGameMode::ResetGame()
- implement all the method from ARgsTileGameMode
- The game should be reset immediately if the player falls off the platform
- Use C++ inheritance to handle ATiles
- Add tile that turns Red
- Add tile that turns Green
- Add tile that turns gray
- Add tile that turns blu
- Generate grid reading input from TileGameModeBP
- handle trigger interactions with ATile classes
- there should be some BP rules that constrain red tile to be a certain number that doesn't prevent player win (there is no path for the player to win)

# Currently working on

- Adding red tiles using similar methods https://www.youtube.com/watch?v=uqylRabVUTo
- Adding green tiles using some rules