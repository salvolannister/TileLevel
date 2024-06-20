# 3D Minesweeper Adventure

## Introduction

Welcome to the **3D Minesweeper Adventure** repository! This project showcases my abilities with Unreal Engine 5 by bringing a new twist to the classic Minesweeper game. In this 3D version, players navigate a grid to collect green tiles, avoid red ones, and uncover the blue tile that reveal the position of the green tiles in the grid, helping them complete the game faster.

### Key Features
- **3D Randomly generated Environment**: Explore the Minesweeper grid in an immersive 3D space.
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
<img src="Images/TileGameGif.gif" alt="Tile Game Preview" width="400"/>

## Implementation

### The tiles

The game includes four types of tiles, each represented by a class: `ARedTile.cpp`, `AGreenTile.cpp`, `ABlueTile.cpp`, and `ANormalTile.cpp`. These classes all derive from a common base class, `ATile.cpp`, and are each linked to a corresponding blueprint. 
Implementing the tiles with a shared base class allows for common behaviors to be centralized, reducing code repetition and speeding up the creation of new `ATile` subclasses.

### The randomly generated environment

To create a randomly generated environment and ensure that colored tiles are placed differently each time the game is replayed, we need an effective randomization method to enhance game replayability. A naive approach might involve selecting a random index within the range [0, TotalTiles - 1] each time we need a colored tile. However, this method has several drawbacks:

- It doesn't guarantee the table is created in a determinate amount of time (we might repeatedly select an already-used index).
- Checking if an index has already been used consumes additional time.
- It might result in a weak randomization.

To overcome these issues, we can use a more robust method. In the RgsPlayGameMode.cpp class, we first create an array of indices where each cell contains its respective position number. We then shuffle the array using a for loop:

- We select a random index between the current loop index "i" and the total count of tiles.
- We swap the elements at array[i] and array[random_index].

This method ensures a well-distributed randomization of tile positions, improving both performance and game replayability.

### Gameplay

For this type of game, the initial instinct to determine which tile the player is on might be to use physics. However, using raycasting and collision detection can be very resource-intensive.
Instead, I developed a method to find the tile position based on the player's position, the location of the tile grid center, the edge dimensions of the grid, and the size of each sector. The formula is as follows:

```cpp
FVector TmpPosition = (Position - TilesGridCenterPosition) / SectorSize + EdgeGridSize;
return FVector2D(FMath::RoundToInt(TmpPosition.X), FMath::RoundToInt(TmpPosition.Y));
```

This approach calculates the tile position efficiently, bypassing the need for heavy physics calculations.

### The UI

## To Do

- Change hard text references 
- Improve the UI interaction between BP and code 
- <b>[Additional]</b> There should be some BP rules that constrain red tiles to be a certain number that doesn't prevent player win (there is no path for the player to win)
- [Additional] Add effect on tile Green StepOn: green tile should tremble and after should show stars and later green color
- [Additional] Add sounds 

## Currently working on

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

 
