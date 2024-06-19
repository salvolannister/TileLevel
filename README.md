# 3D Minesweeper Adventure

## Introduction

Welcome to the **3D Minesweeper Adventure** repository! This project showcases my abilities with Unreal Engine 5 by bringing a new twist to the classic Minesweeper game. In this 3D version, players navigate a grid to collect green tiles, avoid red ones, and uncover the blue tile that reveal the position of the green tiles in the grid, helping them complete the game faster.

### Key Features
- **3D Environment**: Explore the Minesweeper grid in an immersive 3D space.
- **Collectible Tiles**: Strategically collect green tiles to score points.
- **Hazardous Tiles**: Avoid red tiles that can end the game.
- **Reveal Mechanic**: Find blue tiles to reveal the grid and plan your moves.

### Technologies Used
- **Unreal Engine 5**: Utilized for creating a visually stunning and interactive 3D environment.
- **Blueprints and C++**: Combined for game logic and mechanics.

### YouTube Video
<a href="https://www.youtube.com/watch?v=XiqYCUsPEJI" target="_blank">
  <img src="https://img.youtube.com/vi/XiqYCUsPEJI/maxresdefault.jpg" alt="Tile Game video" width="400" style="padding: 10px; border: 1px solid #ddd;"/>
</a>

### GIF Preview
<img src="path/to/your/video-preview.gif" alt="Tile Game Preview" width="400"/>

## Requirements

- Change hard text references 
- Improve the UI interaction between BP and code 
- <b>[Additional]</b> There should be some BP rules that constrain red tiles to be a certain number that doesn't prevent player win (there is no path for the player to win)
- [Additional] Add effect on tile Green StepOn: green tile should tremble and after should show stars and later green color
- [Additional] Add sounds 

## Currently working on

# RGSTileGameMode

- Has the purpose to create the tile grid and set up the info about the tile collected 
- GetClosestTileDistance is the method used to calculate the position closest position for the red and green tiles.
I consider a tile on the diagonal to be at distance 2 since it will require the player to move one time both in x and y dimensions.
- 

# Doubts

- ask someone if the implementation of the HUD half in the BP and half in cpp is a red flag or is ok:
  I did this because it was implemented already in the blueprint, but I wanted to add an animation and the faster way I knew to do it
  was using cpp 
--> ANSWER: just tell them that since it was already half implemented and was a test you left it there but for production you would have move everything inside cpp

- Reloading the scene was already in the code base, but it kinda feels very bad for optimization. Should I remove it?

- If in ATile the basic implementation is absent should I remove it from the cpp file?

- Should I remove this log or set it between #WITH_DEBUG?
	if (EndGameText.IsNull())
	{
		UE_LOG(LogTemp, Error, TEXT(" End Game block text is null"));
		return;
	}

# Resources
- Adding red tiles using similar methods https://www.youtube.com/watch?v=uqylRabVUTo

 
