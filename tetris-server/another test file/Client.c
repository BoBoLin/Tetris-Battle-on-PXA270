#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<time.h>
#include<pthread.h>

int sockfd = 0;
int test = 0;
void send_message(void);
void recv_message(void);

int main(int argc, char *argv[])
{
	if( argv[1] == NULL ) {
		printf("Please enter server's IP address\n");
		return 0;
	}

	//connect to socket
	int port = 1210;
	char *ip = argv[1];
	struct sockaddr_in server_addr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if( sockfd < 0 )
	{
		printf("\nError : Could not create socket \n");
		return;
	}

	memset(&server_addr, '0', sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);

	if( inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0 )
	{
		printf("\ninet_pton error occured\n");
		return;
	}

	if( connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
		printf("\nError : Connect Failed\n");
		return;
	}

	//Receive introducion message from server
	char buffer[1000];
	memset(buffer, '\0', 1000);
	recv(sockfd, buffer, 1000, 0);
	puts(buffer);

	//create thread to send messages
	pthread_t send_pid;
	if( pthread_create(&send_pid, NULL, (void*)send_message, NULL) !=0 )
	{
		printf("Create sending pthread failed!\n");
	}
	
	//create thread to receive messages
	pthread_t recv_pid;
	if( pthread_create(&recv_pid, NULL, (void*)recv_message, NULL) !=0 )
	{
		printf("Create receiving pthread failed!\n");
	}

	while(1) {
		if( test == 999 )
		{
			printf("%d\n", test);
			test = 0;
		}
	}

	pthread_join(send_pid, NULL);
	pthread_join(recv_pid, NULL);
	printf("join");
	close(sockfd);
	return 0;
}


//Create socket, passing parameters as server's IP and port
void send_message(void)
{
	while(1)
	{	
		char recv_msg[1000], send_msg[1000];
	
		//Inplement commends
		memset(send_msg, '\0', 1000);			
		scanf("%s", send_msg);
		send(sockfd, send_msg, strlen(send_msg), 0);
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
		recv(sockfd, recv_msg, 1000,0);
		//puts(recv_msg);

		char command = recv_msg[0];
		switch(command) {
			case 'S':
				test = 999;
				printf("----------------%d--------------------\n", test);
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


