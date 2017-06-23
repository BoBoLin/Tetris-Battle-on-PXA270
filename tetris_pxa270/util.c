/*
 *      util.c
 *      Copyright © 2008 Martin Duquesnoy <xorg62@gmail.com>
 *      All rights reserved.
 */

#include <sys/time.h>

#include "tetris.h"
#include "config.h"

void clear_term(void)
{
     puts("\e[2J");    // clear screem

     return;
}

void set_cursor(Bool b)
{
     printf("\e[?25%c", ((b) ? 'h' : 'l'));

     return;
}

void set_color(int color)
{
     int bg = 0, fg = 0;

     switch(color)
     {
     default:
     case Black:   bg = 0;  break;
     case Blue:    bg = 44; break;
     case Red:     bg = 41; break;
     case Magenta: bg = 45; break;
     case White:   bg = 47; break;
     case Green:   bg = 42; break;
     case Cyan:    bg = 46; break;
     case Yellow:  bg = 43; break;
     case Border:  bg = 47; break;
     case Score:   fg = 37; bg = 49; break;
     }

     printf("\e[%d;%dm", fg, bg);

     return;
}

void printxy(int color, int x, int y, char *str)
{
     set_color(color);
     printf("\e[%d;%dH%s", ++x, ++y, str);
     set_color(0);

     return;
}

int nrand(int min, int max)
{
     return (rand() % (max - min + 1)) + min;
}

void sig_handler(int sig)
{
     switch(sig)
     {
     case SIGTERM:
     case SIGINT:
     case SIGSEGV:
          running = False;
          break;
     case SIGALRM:
          tv.it_value.tv_usec -= tv.it_value.tv_usec / 3000;
          setitimer(0, &tv, NULL);
          break;
     }

     return;
}
