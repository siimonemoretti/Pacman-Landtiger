# Pacman on ARM board
Project for Computer Architecture course at Politecnico di Torino.
Board target of this project is the Landtiger LPC1768.

## Game specifications

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