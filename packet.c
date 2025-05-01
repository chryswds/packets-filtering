// setting:
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h> //inet_addr

int main(int argc, char *argv[]) 
{
	int socket_desc;
	struct sockaddr_in server;
	char *ip_address = "172.217.23.110"; // googles ip address
	int port = 80; //http port
	char *message, server_reply[2000];
	

	//CREATE SOCKET
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	//Address Family - AF_INET (this is IP version 4) 
	//Type - SOCK_STREAM (this means connection oriented TCP protocol) 
	//Protocol - 0 [ or IPPROTO_IP This is IP protocol]
	
	if(socket_desc == -1){
		printf("Could not create socket");
		return 1;
	}
	printf("Socket created successfully\n");

	server.sin_addr.s_addr = inet_addr(ip_address);
	server.sin_family = AF_INET; //IPv4
	server.sin_port = htons(port);

	
	//CONNECT TO REMOTE SERVER
	if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0){
		printf("connect error");
		close(socket_desc);
		return 1;
	}
	printf("connected \n");

	//SEND SOME DATA
	message = "GET / HTTP/1.1\r\n\r\n";//The message is actually a http command to fetch the mainpage of a website.
	if (send(socket_desc , message, strlen(message), 0) < 0){
		printf("Send failed");
		return 1;
	}
	printf("Data Send\n");


	//RECEIVE A REPLY FROM THE SERVER
	if( recv(socket_desc, server_reply, 2000, 0) < 0){
		printf("recv failed");
	}
	printf("Reply received\n");
	puts(server_reply);

	return 0;

};



