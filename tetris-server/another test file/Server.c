#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/stat.h>

#include<unistd.h>
#include<errno.h>

#include<pthread.h>

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

int connfd = 0;
void recv_message(void);
void send_message(void);

int main()
{
	//create socket
	int listenfd = 0, port = 1210;
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
	pthread_t recv_pid;
	if( pthread_create(&recv_pid, NULL, (void*)recv_message, NULL) !=0 )
	{
		printf("Create receivng pthread failed!\n");
	}

	//create thread to send messages
	pthread_t send_pid;
	if( pthread_create(&send_pid, NULL, (void*)send_message, NULL) !=0 )
	{
		printf("Create sending pthread failed!\n");
	}

	while(1) {

	}

	pthread_join(recv_pid, NULL);
	pthread_join(send_pid, NULL);
	printf("join");
	close(connfd); 
	return 0;
}


void recv_message(void)
{
	while(1)
	{
		char *send_msg, recv_msg[1000];	
		memset(recv_msg, '\0', 1000);
		recv(connfd, recv_msg, 1000,0);

		char command = recv_msg[0];
		switch(command) {
			case 'S':
				printf("Start\n");
				break;
			case 'E':
				printf("Game over\n");
				break;
			case 'N':
				printf("New line\n");
				break;
			default:
				puts(recv_msg);
				break;
		}
	}
	return;
}

void send_message(void)
{
	printf("Start sending messages when keyboard input\n\n");
	while(1)
	{	
		//Some data to store files and messages 
		char recv_msg[1000], send_msg[1000];
	
		//Inplement commends
		memset(send_msg, '\0', 1000);			
		scanf("%s", send_msg);
		send(connfd, send_msg, strlen(send_msg), 0);
	}
	return;
}


