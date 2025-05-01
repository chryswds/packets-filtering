// setting:
#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h> //inet_addr

char *ip_address(){
	char *hostname = "www.google.com";
	static char ip[100];
	// hostent - The ip addresses of a hostname. Used by gethostbyname
	struct hostent *he;
	// in_addr - Ip address in long format
	struct in_addr **addr_list;
	int i;

	if ((he = gethostbyname( hostname )) == NULL){
		herror("gethostbyname");
		return NULL;
		
	}

	addr_list = (struct in_addr **) he->h_addr_list;

	//Function inet_ntoa will convert an IP address in long format to dotted format. This is just the opposite of inet_addr.
	for(i = 0; addr_list[i] != NULL; i++){
		strcpy(ip, inet_ntoa(*addr_list[i]));
	}
	return ip;
}

int main(int argc, char *argv[]) 
{
	int socket_desc;
	// sockaddr_in - Connection information. Used by connect , send , recv etc.
	struct sockaddr_in server;
	char *ip_addresses = ip_address(); // googles ip address
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

	server.sin_addr.s_addr = inet_addr(ip_addresses);
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
	
	close(socket_desc);

	return 0;

};
