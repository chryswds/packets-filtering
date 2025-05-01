// setting:
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h> //inet_addr

int main(int argc, char *argv[]) 
{
	int socket_desc;
	struct sockaddr_in server;
	char *ip_address = "172.217.23.110"; // googles ip address
	int port = 80; //http port
	
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

	
	//Connect to remote server
	if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0){
		printf("connect error");
		close(socket_desc);
		return 1;
	}
	printf("connected \n");
	return 0;

};



