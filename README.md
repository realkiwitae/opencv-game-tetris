# opencv-game-tetris
Tetris game using c++

# Game Reference
https://en.wikipedia.org/wiki/Tetris

# Requirements
User can start the game and restart after a win/loss

# Requirements (Win/Lose)
Win: achieve N score to win
Lose: stack up to the top of the playing field

# Requirements (UI)
Current score, Menu to start/restart game.

# Requirements
Tetromino: composed of four squares, at least 3 tetrominoes(e.g. I, O, T tetromino refer from: https://en.wikipedia.org/wiki/Tetromino)
Each tetromino moves downward slowly by itself
Filled rows will clear, adding to the score

# Requirements
Input:
Down Key: Make tetromino move downwards faster
Left/Right Key: Make tetromino move left/right
Up Key: Undefined (In original design it is to rotate tetromino ninety degrees, but we don't require this feature.)
Others:
You can choose the field dimension, it doesn't matters. The classic spec is 10 cells wide by 20 high. smaller or bigger is ok.

# Bonus Features (optional)
Hold piece: Press a key to do Hold/Swap tetromino. Hold piece - TetrisWiki 
Ghost piece: Ghost piece | Tetris Wiki | Fandom 