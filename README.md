# Tetris-Battle-on-PXA270

PXA270 acts as a server</br>
PC acts as a client

Let's play the Tetris on the computer and the PXA270

![alt text](https://github.com/BoBoLin/Tetris-Battle-on-PXA270/blob/master/simple%20structure.PNG)

### Design & Implementation

##### Socket Connection
+ Starting server when executing program
+ Pressing button to start game after connection
+ Adding a line to opponent’s screen when clearing  lines
+ Game over when one of player lose

#### Thread Implementation
+ One thread for playing game
+ One thread for socket receiving
+ One thread for socket sending


#### Tetris Battle Game on PC
+ Colored blocks  
+ Scores  
+ Seeing next blocks  
+ Buttons  
> Rotate :2、5  
> Move : 4、6  
> Straight down : 7  
> Speed up : C  
> Pause : 8  
> Quit : 9  

#### Tetris Battle Game on PXA270
+ Scores  
+ Seeing next blocks  
+ Buttons  
> Rotate :2、5  
> Move : 4、6  
> Straight down : 7  
> Speed up : C  
> Pause : 8  
> Quit : 9  

### Differences between PC and PXA270
+ Only three types of uncolored blocks  
> Border  
> General blocks  
> Blocks added by opponent  
+ Changing multiplication to shift operation
+ Renewing specific places during landing of the same block to reduce CPU execution

