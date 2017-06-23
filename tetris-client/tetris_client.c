#include "tetris.h"
#include "config.h"

/* Functions */
void init(void)
{
     
     struct sigaction siga;
     struct termios term;

     /* Clean term */
     clear_term();
     set_cursor(False);

     /* Make rand() really random :) */
     srand(getpid());

     /* Init variables */
     score = lines = 0;
     running = True;
     current.y = (FRAMEW / 2) - 1;
     current.num = nrand(0, 6);
     current.next = nrand(0, 6);

     /* Score */
     printxy(0, FRAMEH_NB + 2, FRAMEW + 3, "Score:");
     printxy(0, FRAMEH_NB + 3, FRAMEW + 3, "Lines:");
     DRAW_SCORE();

     printxy(0, FRAMEH_NB + 5, FRAMEW + 3, "Opponent's score:");
     printxy(0, FRAMEH_NB + 6, FRAMEW + 3, "0");

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
          case KEY_SPEED:                ++current.x; DRAW_SCORE(); break;
          case KEY_PAUSE:                while(getchar() != KEY_PAUSE);      break;
          case KEY_QUIT:                 running = False;GameOver = 1;        break;
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
               if((frame[i][j] == 0) || (frame[i][j] == 11))
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
          client_cmd = 2;
     arrange_score(nl);
     frame_refresh();

     return;
}

void plus_one_line(void)
{
     int i, j, k;

     for(i = 1 ;i< FRAMEH-1; ++i)
     {
          for(j = 2; j < FRAMEW - 1; ++j)
               frame[i][j] = frame[i+1][j];
     }
     for(k = 2; k < FRAMEW-1; ++k)
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

     //pthread_join(send_pid, NULL);
     //pthread_join(recv_pid, NULL);
     
     return;
}

//Create socket, passing parameters as server's IP and port
void send_message(void)
{
     while(1)
     {    
          if(client_cmd == 0) continue;
          else if(client_cmd == 2)
          {
               char send_msg[1000];
               //Inplement commends
               memset(send_msg, '\0', 1000);
               send_msg[0] = 'N';
               char str[10];
               sprintf(str, "%d", score);  // int to char
               //printf("send score: %s\n", str);
               //send_msg[1] = str;
               int i;
               for(i = 1; i <= strlen(str); i++)
                    send_msg[i] = str[i-1];
               send(connfd, send_msg, strlen(send_msg), 0);
               client_cmd = 0;
          }
          else if(client_cmd == 3)
          {
               char send_msg[1000];
               //Inplement commends
               memset(send_msg, '\0', 1000);
               send_msg[0] = 'E';
               send(connfd, send_msg, strlen(send_msg), 0);
               client_cmd = 0;
          }
          if(GameOver == 1)
               break;
     }
     return;
}

void recv_message(void)
{
     printf("Start receiving messages from server\n\n");
     while(1)
     {    
          //Some data to store files and messages 
          char recv_msg[1000];

          //Receive commands from server
          memset(recv_msg, '\0', 1000);
          recv(connfd, recv_msg, 1000,0);
          //puts(recv_msg);

          char command = recv_msg[0];
          switch(command) {
               case 'Z':
                    client_cmd = 1;
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
          
          if( recv_msg[1] != '0')
          {
               for(i = 1; i< strlen(recv_msg); i++)
                    op_score[i-1] = recv_msg[i];
          }

          //printxy(0, FRAMEH_NB + 6, FRAMEW + 3, op_score);

          if(GameOver == 1)
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
     client_cmd = 0;

     if( argv[1] == NULL ) {
          printf("Please enter server's IP address\n");
          return 0;
     }

     //connect to socket
     int port = 3100;
     char *ip = argv[1];
     struct sockaddr_in server_addr;

     connfd = socket(AF_INET, SOCK_STREAM, 0);
     if( connfd < 0 )
     {
          printf("\nError : Could not create socket \n");
          return 0;
     }

     memset(&server_addr, '0', sizeof(server_addr));
     server_addr.sin_family = AF_INET;
     server_addr.sin_port = htons(port);

     if( inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0 )
     {
          printf("\ninet_pton error occured\n");
          return 0;
     }

     if( connect(connfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
     {
          printf("\nError : Connect Failed\n");
          return 0;
     }

     //Receive introducion message from server
     char buffer[1000];
     memset(buffer, '\0', 1000);
     recv(connfd, buffer, 1000, 0);
     puts(buffer);

     //create thread to send messages
     //pthread_t send_pid;
     if( pthread_create(&send_pid, NULL, (void*)send_message, NULL) !=0 )
     {
          printf("Create sending pthread failed!\n");
          return 0;
     }
     
     //create thread to receive messages
     //pthread_t recv_pid;
     if( pthread_create(&recv_pid, NULL, (void*)recv_message, NULL) !=0 )
     {
          printf("Create receiving pthread failed!\n");
          return 0;
     }

     //int s = getchar();
     
     while(client_cmd != 1)
     {

     }

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
               NewLine = 0;
          }
          //i++;
          //printf("counter : %d", i);
     }
     client_cmd = 3;
     
     //printf("GameOver:%d\n", GameOver);
     quit();
     sleep(2500);
     close(connfd);
     exit(0);

     return 0;
}
