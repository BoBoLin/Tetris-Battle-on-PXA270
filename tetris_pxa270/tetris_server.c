#include "tetris.h"
#include "config.h"

/* Functions */
void init(void)
{
     struct sigaction siga;
     struct termios term;
	fd = open("/dev/lcd", O_RDWR);

     /* Clean term */
     clear_term();
     set_cursor(False);

     /* Make rand() really random :) */
     srand(getpid());

     /* Init variables */
     score = lines = 0;
     running = True;
     current.y = (FRAMEW / 2) - 1;
     current.num = nrand(3, 4);
     current.next = nrand(0, 6);

     /* Score */
     printxy(0, FRAMEH_NB + 2, FRAMEW + 3, "Score:");
     printxy(0, FRAMEH_NB + 3, FRAMEW + 3, "Lines:");
     DRAW_SCORE();

     LCD_Cursor(fd, FRAMEW+2, FRAMEH_NB + 3);
     LCD_fprintf(fd, "%c", '0');
     LCD_Cursor(fd, 14, 15);

     /* Init signal */
     sigemptyset(&siga.sa_mask);
     siga.sa_flags = 0;
     siga.sa_handler = sig_handler;
     sigaction(SIGALRM, &siga, NULL);
     sigaction(SIGTERM, &siga, NULL);
     sigaction(SIGINT,  &siga, NULL);
     sigaction(SIGSEGV, &siga, NULL);

     /* Init timer */
     tv.it_value.tv_usec = TIMING;
     sig_handler(SIGALRM);

     /* Init terminal (for non blocking & noecho getchar(); */
     tcgetattr(STDIN_FILENO, &term);
     tcgetattr(STDIN_FILENO, &back_attr);
     term.c_lflag &= ~(ICANON|ECHO);
     tcsetattr(0, TCSANOW, &term);

     return;
}

void get_key_event(void)
{
	
     int c = getchar();

     if(c > 0)
     {
          //printf("c\n");
          --current.x;
     }

     switch(c)
     {
          case KEY_MOVE_LEFT:            shape_move(-EXP_FACT);           break;
          case KEY_MOVE_RIGHT:           shape_move(EXP_FACT);           break;
          case KEY_CHANGE_POSITION_NEXT: shape_set_position(N_POS);         break;
          case KEY_CHANGE_POSITION_PREV: shape_set_position(P_POS);       break;
          case KEY_DROP_SHAPE:           shape_drop();                   break;
          case KEY_SPEED:                ++current.x; ++score; DRAW_SCORE(); break;
          case KEY_PAUSE:                while(getchar() != KEY_PAUSE);      break;
          case KEY_QUIT:                 running = False;GameOver = 1;       break;
     }
	
	unsigned short key;
	if(KEYPAD_get_key(fd, &key) == OK) {
		switch(key)
		{
			case KEY_MOVE_LEFT:            printf("left");shape_move(-EXP_FACT);           break;
			case KEY_MOVE_RIGHT:           printf("right");shape_move(EXP_FACT);           break;
			case KEY_CHANGE_POSITION_NEXT: printf("up");shape_set_position(N_POS);         break;
			case KEY_CHANGE_POSITION_PREV: printf("down");shape_set_position(P_POS);       break;
			case KEY_DROP_SHAPE:           printf("space");shape_drop();                   break;
			case KEY_SPEED:                printf("s");++current.x;DRAW_SCORE();           break;
			case KEY_PAUSE:                printf("p");while(1){if(KEYPAD_get_key(fd, &key)==OK)if(key==KEY_PAUSE)break;};      break;
			case KEY_QUIT:                 printf("q");running = False;GameOver = 1;       break;
		}
	}

     return;
}

void arrange_score(int l)
{
     /* Standard score count */
     switch(l)
     {
          case 1: score += 10;   break; /* One line */
          case 2: score += 40;  break; /* Two lines */
          case 3: score += 90;  break; /* Three lines */
          case 4: score += 160; break; /* Four lines */
     }

     lines += l;

     DRAW_SCORE();

     return;
}

void check_plain_line(void)
{
     int i, j, k, f, g, h, l, c = 0, nl = 0;

     for(i = 1; i < FRAMEH; ++i)
     {
          for(j = 1; j < FRAMEW; ++j)
               if((frame[i][j] == 0)|| (frame[i][j] == 11))
                    ++c;
          if(!c)
          {
               ++nl;
               for(k = i - 1; k > 1; --k)
                    for(f = 1; f < FRAMEW; ++f)
                         frame[k + 1][f] = frame[k][f];

	       for(g = i+1; g < FRAMEH; ++g)
               {
                    if(frame[g][3] == 11)
                    {
                         for(h = g - 1; h > 1; --h)
                              for(l = 1; l < FRAMEW; ++l)
                                   frame[h + 1][l] = frame[h][l];

                         break;
                    }
               }
          }
          c = 0;
     }
     if(nl > 0)
          server_cmd = 2;
     arrange_score(nl);
     frame_refresh();

     return;
}

void plus_one_line(void)
{
     int i, j, k;

     for(i = 1 ;i< FRAMEH-1; ++i)
     {
          for(j = 1; j < FRAMEW; ++j)
               frame[i][j] = frame[i+1][j];
     }
     for(k = 1; k < FRAMEW; ++k)
          frame[FRAMEH-1][k] = 11;
     return;
}

int check_possible_pos(int x, int y)
{
     int i, j, c = 0;

     for(i = 0; i < 4; ++i)
          for(j = 0; j < EXP_FACT; ++j)
               if(frame[x + shapes[current.num][current.pos][i][0]]
                  [y + shapes[current.num][current.pos][i][1] * EXP_FACT + j] != 0)
                    ++c;

     return c;
}

void quit(void)
{
     frame_refresh(); /* Redraw a last time the frame */
     set_cursor(True);
     tcsetattr(0, TCSANOW, &back_attr);
     printf("\nGame Over\nYour score: %d\nOpponent's score: %s\n", score, op_score);
     char str[10];
     sprintf(str, "%d", score);  // int to char

     LCD_Cursor(fd, 2, 2);
     LCD_fprintf(fd,"\nGame Over\nYour score: %s\nOp's score: %s\n", str, op_score);
     LCD_Cursor(fd, 14, 15);

     return;
}

//Create socket, passing parameters as server's IP and port
void send_message(void)
{
     printf("Start sending messages when keyboard input\n\n");
     printf("Enter 'z' to start game together!\n\n");
     while(1)
     {    
          if(server_cmd == 0) continue;
          else if(server_cmd == 1)
          {
               //Some data to store files and messages 
               char send_msg[1000];
          
               //Inplement commends
               memset(send_msg, '\0', 1000);
               send_msg[0] = 'Z';
               send(connfd, send_msg, strlen(send_msg), 0);
               server_cmd = 0;
          }
          else if(server_cmd == 2)
          {
               //Some data to store files and messages 
               char send_msg[1000];
               
               //Inplement commends
               memset(send_msg, '\0', 1000);
               send_msg[0] = 'N';
               char str[10];
               sprintf(str, "%d", score);  // int to char

               int i;
               for(i = 1; i <= strlen(str); i++)
                    send_msg[i] = str[i-1];
               
               send(connfd, send_msg, strlen(send_msg), 0);
               server_cmd = 0;
          }
          else if(server_cmd == 3)
          {
               //Some data to store files and messages 
               char send_msg[1000];
               
               //Inplement commends
               memset(send_msg, '\0', 1000);
               send_msg[0] = 'E';
               send(connfd, send_msg, strlen(send_msg), 0);
               server_cmd = 0;
          }

          if(GameOver==1)
               break;
     }
     return;
}

void recv_message(void)
{
     while(1)
     {
          char recv_msg[1000];    
          memset(recv_msg, '\0', 1000);
          recv(connfd, recv_msg, 1000,0);

          char command = recv_msg[0];
          switch(command) {
               case 'S':
                    printf("Start\n");
                    break;
               case 'E':
                    //printf("Game over\n");
                    GameOver = 1;
                    break;
               case 'N':
                    NewLine = 1;
                    break;
               default:
                    puts(recv_msg);
                    break;
          }
          
          int i ;
          //for(i = 0; i< )
          //char op_score = recv_msg[1];
          if( recv_msg[1] != '0')
          {
               for(i = 1; i< strlen(recv_msg); i++)
                    op_score[i-1] = recv_msg[i];
          }

          if(GameOver ==1)
               break;

     }
     return;
}

int main(int argc, char **argv)
{
     memset(op_score, '\0', sizeof(op_score));
     get_lines = 0;
     NewLine = 0;
     GameOver = 0;
     connfd = 0;
     server_cmd = 0;

     printf("You are Server , waiting for client connecting......\n");
     //create socket
     int listenfd = 0, port = 3100;
     struct sockaddr_in server_addr;
     
     listenfd = socket(AF_INET, SOCK_STREAM, 0);
     memset(&server_addr, '0', sizeof(server_addr));   

     server_addr.sin_family = AF_INET;
     server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
     server_addr.sin_port = htons(port);

     bind(listenfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
     listen(listenfd, 10);

     connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);

     //Send introducion message to client    
     char *intro = "\nWelcome to Server\0";
     if( send(connfd, intro, strlen(intro), 0) >= 0 )
          printf("\nA client has connected.\n");

     //create thread to receive messages
     //pthread_t recv_pid;
     if( pthread_create(&recv_pid, NULL, (void*)recv_message, NULL) !=0 )
     {
          printf("Create receivng pthread failed!\n");
          return 0;
     }

     //create thread to send messages
     //pthread_t send_pid;
     if( pthread_create(&send_pid, NULL, (void*)send_message, NULL) !=0 )
     {
          printf("Create sending pthread failed!\n");
          return 0;
     }

     printf("socket thread success\n");

     int z = getchar();

     while(z != 'z')
     {
          z = getchar();
          if(z == 'q')
               return 0;
     }

     server_cmd = 1;  //server start

     init();

     frame_init();
     frame_nextbox_init();

     current.last_move = False;


     //int i =0;
     while(running)
     {
          get_key_event();
          shape_set();
          frame_refresh();
          shape_go_down();
          if(GameOver==1)
               break;
          if(NewLine ==1)
          {
               plus_one_line();
               get_lines++;
               char str[10];
               char str_print[1024]= "You got ";
               sprintf(str, "%d", get_lines);
               strcat(str_print, str);
               char str_print2[32] = " lines !\n";
               strcat(str_print, str_print2);
               printxy(0, FRAMEH_NB + 9, FRAMEW + 3, str_print);
               printxy(0, FRAMEH_NB + 6, FRAMEW + 3, op_score);
		
		LCD_Cursor(fd, FRAMEW+2, FRAMEH_NB + 3);
		LCD_fprintf(fd, "%s", op_score);
		LCD_Cursor(fd, 14, 15);
		
               NewLine = 0;
          }
          //i++;
          //printf("counter : %d", i);
     }
     server_cmd = 3;
     
     //printf("GameOver:%d\n", GameOver);
     quit();
     sleep(2500);
     close(connfd);
     exit(0);

     return 0;
}

