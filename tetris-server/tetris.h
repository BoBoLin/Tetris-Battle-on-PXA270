/* Libs */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <termios.h>
#include <sys/time.h>

#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <pthread.h>


/* Expension factor of shapes */
#define EXP_FACT 2

/* Frame dimension */
#define FRAMEW (int)(23)
#define FRAMEH (int)(20)
#define FRAMEW_NB 15
#define FRAMEH_NB 5

/* Shapes position */
#define N_POS ((current.pos < 3) ? current.pos + 1 : 0)
#define P_POS ((current.pos > 0) ? current.pos - 1 : 3)

/* Draw the score.. */
#define DRAW_SCORE() set_color(Score);                             \
     printf("\033[%d;%dH %d", FRAMEH_NB + 3, FRAMEW + 10, score);   \
     printf("\033[%d;%dH %d", FRAMEH_NB + 4, FRAMEW + 10, lines);   \
     set_color(0);

/* Bool type */
typedef enum { False, True } Bool;

/* Shape structure */
typedef struct
{
     int num;
     int next;
     int pos;
     int x, y;
     Bool last_move;
} shape_t;

/* Color enum */
enum { Black, Blue, Red, Magenta, White, Green, Cyan, Yellow, Border, Score, ColLast , DarkBlack};

/* Prototypes */

/* util.c */
void clear_term(void);
void set_cursor(Bool);
void printxy(int, int, int, char*);
void set_color(int);
int nrand(int, int);
void sig_handler(int);

/* frame.c */
void frame_init(void);
void frame_nextbox_init(void);
void frame_refresh(void);
void frame_nextbox_refresh(void);

/* shapes.c */
void shape_set(void);
void shape_unset(void);
void shape_new(void);
void shape_go_down(void);
void shape_set_position(int);
void shape_move(int);
void shape_drop(void);

/* tetris.c */
void arrange_score(int l);
void check_plain_line(void);
int check_possible_pos(int, int);
void get_key_event(void);
void plus_one_line(void);
/* Variables */

const int shapes[7][4][4][2];
struct itimerval tv;
struct termios back_attr;
shape_t current;
int frame[FRAMEH + 1][FRAMEW + 1];
int frame_nextbox[FRAMEH_NB][FRAMEW_NB];
int score;
int lines;
Bool running;
int GameOver;


int connfd;
void recv_message(void);
void send_message(void);

int server_cmd;
int client_cmd;
pthread_t recv_pid;
pthread_t send_pid;

int NewLine;
int get_lines;

char op_score[10];
