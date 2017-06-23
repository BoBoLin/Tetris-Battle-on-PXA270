# Tetris-Battle-on-PXA270

PXA270 acts as a server
PC acts as a client

Let's play the Tetris on the computer and the PXA270

<h3>Design & Implementation</h2>

<h4>Socket Connection</h4>
<ul>
    <li>Starting server when executing program</li>
    <li>Pressing button to start game after connection</li>
    <li>Adding a line to opponent’s screen when clearing  lines</li>
    <li>Game over when one of player lose</li>
</ul>

  
<h4>Thread Implementation</h4>
<ul>
    <li>One thread for playing game</li>
    <li>One thread for socket receiving</li>
    <li>One thread for socket sending</li>
</ul>


<h4>Tetris Battle Game on PC</h4>
<ul>
    <li>Colored blocks</li>
    <li>Scores</li>
    <li>Seeing next blocks</li>
    <li>Buttons</li>
</ul>

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
