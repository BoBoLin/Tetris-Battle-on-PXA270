/*
 *      shapes.c
 *      Copyright © 2008 Martin Duquesnoy <xorg62@gmail.com>
 *      All rights reserved.
 */

#include "tetris.h"

/* Shapes data */
const int shapes[7][4][4][2] =
{
     /* O */
     {
          {{0,0},{1,0},{0,1},{1,1}},
          {{0,0},{1,0},{0,1},{1,1}},
          {{0,0},{1,0},{0,1},{1,1}},
          {{0,0},{1,0},{0,1},{1,1}}
     },
     /* I */
     {
          {{1,0},{1,1},{1,2},{1,3}},
          {{0,1},{1,1},{2,1},{3,1}},
          {{1,0},{1,1},{1,2},{1,3}},
          {{0,1},{1,1},{2,1},{3,1}}
     },
     /* L */
     {
          {{0,1},{1,1},{2,1},{2,2}},
          {{1,0},{1,1},{1,2},{2,0}},
          {{0,0},{0,1},{1,1},{2,1}},
          {{1,0},{1,1},{1,2},{0,2}}
     },
     /* J */
     {
          {{1,0},{1,1},{1,2},{2,2}},
          {{0,2},{1,2},{2,2},{2,1}},
          {{0,0},{1,0},{1,1},{1,2}},
          {{0,1},{0,2},{1,1},{2,1}}
     },
     /* S */
     {
          {{1,1},{1,2},{2,0},{2,1}},
          {{0,1},{1,1},{1,2},{2,2}},
          {{1,1},{1,2},{2,0},{2,1}},
          {{0,1},{1,1},{1,2},{2,2}}
     },
     /* Z */
     {
          {{0,0},{0,1},{1,1},{1,2}},
          {{0,2},{1,1},{2,1},{1,2}},
          {{0,0},{0,1},{1,1},{1,2}},
          {{0,2},{1,1},{2,1},{1,2}}
     },
     /* T */
     {
          {{0,1},{1,0},{1,1},{1,2}},
          {{0,1},{1,1},{1,2},{2,1}},
          {{1,0},{1,1},{1,2},{2,1}},
          {{1,0},{0,1},{1,1},{2,1}}
     }
};

void shape_set(void)
{
     int i, j;

     for(i = 0; i < 4; ++i)
          for(j = 0; j < EXP_FACT; ++j)
               frame[current.x + shapes[current.num][current.pos][i][0]]
                    [current.y + shapes[current.num][current.pos][i][1] * EXP_FACT + j]
                    = 1;

     if(current.x < 1)
          for(i = 0; i < FRAMEW + 1; ++i)
               frame[0][i] = Border;

     return;
}

void shape_unset(void)
{
     int i, j;

     for(i = 0; i < 4; ++i)
          for(j = 0; j < EXP_FACT; ++j)
               frame[current.x + shapes[current.num][current.pos][i][0]]
                    [current.y + shapes[current.num][current.pos][i][1] * EXP_FACT + j] = 0;

     if(current.x < 1)
          for(i = 0; i < FRAMEW + 1; ++i)
               frame[0][i] = Border;
     return;
}

void shape_new(void)
{
     int i;

     /* Draw the previous shape for it stay there */
     shape_set();
     check_plain_line();

     /* Set the new shape property */
     current.num = current.next;
     current.x = 1;
     current.y = (FRAMEW / 2) - 1;;
     current.next = nrand(0, 6);

     frame_nextbox_refresh();

     if(current.x > 1)
          for(i = 2; i < FRAMEW - 1; ++i)
               frame[1][i] = 0;

     return;
}

void shape_go_down(void)
{

     shape_unset();

     /* Fall the shape else; collision with the ground or another shape
      * then stop it and create another */
     if(!check_possible_pos(current.x + 1, current.y))
          ++current.x;
     else
          if(current.x > 2)
               shape_new();
          else
          {
               shape_new();
               frame_refresh();
               sleep(2);
               running = False;
          }




     return;
}

void shape_set_position(int p)
{
     int old = current.pos;

     shape_unset();

     current.pos = p ;

     if(check_possible_pos(current.x, current.y))
          current.pos = old;

     return;
}


void shape_move(int n)
{

     shape_unset();

     if(!check_possible_pos(current.x, current.y + n))
          current.y += n;

     return;
}

void shape_drop(void)
{
     while(!check_possible_pos(current.x + 1, current.y))
     {
          shape_unset();
          ++current.x;
     }
     
     return;
}
