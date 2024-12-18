# Pacman on ARM board
Project for Computer Architecture course at Politecnico di Torino.
Board target of this project is the Landtiger LPC1768.

## Part 1 Game specifications

1. Fill labyrinth with 240 standard pills, which are worth 10 points each.
2. Randomly place 6 power pills, which are worth 50 points each. Randomness must be implemented for both position and time of appearance.
3. Player moves Pacman through the labyrinth using the joystick. After direction is chosen, Pacman moves in that direction until it hits a wall or the joystick is moved again.
4. When Pacman reaches the "teleport" tile, it is teleported to the other side of the labyrinth.
5. Every 1000 points, the player gets an extra life.
6. Button 0 switches the game between paused and running states. When paused, the game must display a message on the screen. When the board is reset, the game must start in paused state.
7. There's a timer of 60 seconds. When the timer reaches 0, the game ends.
8. Game ends when:
    - Pacman eats all pills. "Victory" screen is displayed.
    - Timer reaches 0. "Game Over" message is displayed.

## Part 2 Game demo

## Part 2 Game specifications

Extend the game with the following features:

1. Add Blinky (red ghost) to the game. Blinky is spawned at the center of the map and moves with the A* algorithm. His goal is to chase pacman and eat him, however his bevhaviour changes based on game mode:
    - **Chase mode**: Blinky moves towards Pacman's position.
    - **Frightened mode**: Blinky moves away from Pacman's position. This happens when Pacman eats a power pill. The ghost becomes Blue for 10 seconds, runs away from Pacman and can be eaten by him. In this case, pacman earns 100 points and the ghost is respawned at the center of the map after 3 seconds.
2. Configure the speaker to play background music and sound effects.
3. Use the CAN peripheral to print the score of the game, the remaining lifes and the countdown timer. We must send this packet:
    - 1 byte: countdown timer
    - 1 byte: remaining lifes
    - 2 bytes: score