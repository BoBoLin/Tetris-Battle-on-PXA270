/*
 *      frame.c
 *      Copyright © 2008 Martin Duquesnoy <xorg62@gmail.com>
 *      All rights reserved.
 */

#include "tetris.h"

/* Shape attribute for draw it in the next box (center etc..)
 * [0]: +x
 * [1]: +y
 * [2]: What shape position choose for a perfect position in the box
 */
const int sattr[7][3] = {{0,2}, {-1,0}, {-1,1,1}, {-1,1}, {-1,1}, {0,1}, {0,1}};

void frame_init(void)
{
     int i;

     /* Frame border */
     for(i = 0; i < FRAMEW + 1; ++i)
     {
          frame[0][i] = Border;
          frame[FRAMEH][i] = Border;
     }
     for(i = 0; i < FRAMEH; ++i)
     {
          frame[i][0] = Border;
          //frame[i][1] = Border;
          frame[i][FRAMEW] = Border;
          //frame[i][FRAMEW - 1] = Border;
     }

     frame_refresh();

     return;
}

void frame_nextbox_init(void)
{
     int i;
	/*
     for(i = 0; i < FRAMEH_NB; ++i)
     {
          frame_nextbox[i][0] = Border;
          frame_nextbox[i][1] = Border;
          frame_nextbox[i][FRAMEW_NB - 1] = Border;
          frame_nextbox[i][FRAMEW_NB] = Border;

     }
     for(i = 0; i < FRAMEW_NB + 1; ++i)
          frame_nextbox[0][i] = frame_nextbox[FRAMEH_NB][i] = Border;
	*/

     frame_nextbox_refresh();

     return;
}

void frame_refresh(void)
{
     int i, j;
	char c = 'O', d = ' ', e = 'I', x = 'X';

     for(i = 0; i < FRAMEH + 1; ++i)
          for(j = 0; j < FRAMEW + 1; ++j)
		if(frame[i][j] == Border){
			LCD_Cursor(fd, j, i);
			LCD_fprintf(fd, "%c", e);
			LCD_Cursor(fd, 14, 15);
		}else if(frame[i][j] == 0){
			LCD_Cursor(fd, j, i);
			LCD_fprintf(fd, "%c", d);
			LCD_Cursor(fd, 14, 15);
		}else if(frame[i][j] == 1){
			LCD_Cursor(fd, j, i);
			LCD_fprintf(fd, "%c", c);
			LCD_Cursor(fd, 14, 15);
		}else if(frame[i][j] == 11){
			LCD_Cursor(fd, j, i);
			LCD_fprintf(fd, "%c", x);
			LCD_Cursor(fd, 14, 15);
		}

                //printxy(frame[i][j], i, j, " ");
     return;
}

void frame_nextbox_refresh(void)
{
     int i, j;

     /* Clean frame_nextbox[][] */
     for(i = 0; i < FRAMEH_NB; ++i)
          for(j = 0; j < FRAMEW_NB - 1; ++j)
               frame_nextbox[i][j] = 0;

     /* Set the shape in the frame */
     for(i = 0; i < 4; ++i)
          for(j = 0; j < EXP_FACT; ++j)
               frame_nextbox
                    [shapes[current.next][sattr[current.next][2]][i][0] + sattr[current.next][0]]
                    [shapes[current.next][sattr[current.next][2]][i][1] * EXP_FACT + j + sattr[current.next][1]]
                    = 1;

     /* Draw the frame */
	char c = 'o', d = ' ';
     for(i = 0; i < FRAMEH_NB + 1; ++i)
          for(j = 0; j < FRAMEW_NB + 1; ++j)
		if(frame_nextbox[i][j]==1){
			LCD_Cursor(fd, j + FRAMEW+1, i+2);
			LCD_fprintf(fd, "%c", c);
			LCD_Cursor(fd, 14, 15);
		}else{
			LCD_Cursor(fd, j + FRAMEW+1, i+2);
			LCD_fprintf(fd, "%c", d);
			LCD_Cursor(fd, 14, 15);
		}

			
               //printxy(frame_nextbox[i][j], i, j + FRAMEW + 3, " ");

     return;
}


