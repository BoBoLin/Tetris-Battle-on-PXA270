# Tetris-Battle-on-PXA270

PXA270 acts as a server
PC acts as a client

Let's play the Tetris on the computer and the PXA270

<h3>Design & Implementation</h2>

<h4>Socket Connection</h4>
  - Starting server when executing program
  - Pressing button to start game after connection
  - Adding a line to opponent’s screen when clearing  lines
  - Game over when one of player lose
  
<h4>Thread Implementation</h4>
  - One thread for playing game
  - One thread for socket receiving
  - One thread for socket sending

<h4>Tetris Battle Game on PC</h4>
  - Colored blocks
  - Scores
  - Seeing next blocks
  - Buttons
     + Rotate :↑↓
     + Move : ←→
     + Straight down : space
     + Speed up : s
     + Pause : p
     + Quit : q
