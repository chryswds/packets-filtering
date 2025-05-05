#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include <pthread.h>

void *connection_handler(void *);

int main(int argc, char *argv[]){
	int socket_desc, new_socket, c, *new_sock;
	struct sockaddr_in server, client;
	char *message;

	//Create socket
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_desc == -1){
		printf("Could not create socket");
	}


	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( 8888 );


	//Bind
	if ( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) <0){
		puts("bind failed");
		return 1;
	}

	puts("bind done");

	listen(socket_desc, 3);

	//Accepts connection
	puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);
	while( (new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
	{
		puts("Connection accepted");
		
		//Reply to the client
		message = "Hello Client , I have received your connection. And now i will assign a handler for you\n";
		write(new_socket , message , strlen(message));

		pthread_t sniffer_thread;
		new_sock = malloc(1);
		*new_sock = new_socket;


		if ( pthread_create( &sniffer_thread, NULL, connection_handler , (void*) new_sock) < 0){
			perror("could not create thread");
			return 1;
		}

	pthread_join( sniffer_thread, NULL);
		puts("Handler assigned");
	}
	
	if (new_socket<0)
	{
		perror("accept failed");
		return 1;
	}

	return 0;
}

void *connection_handler(void *socket_desc)
{

	int sock = *(int*)socket_desc;
	int read_size;

	char *message, client_message[2000];


	message = "Greetings! I am your connection handler\n";
	write(sock, message, strlen(message));

	message = "Type somethiing\n";
	write(sock, message, strlen(message));

	while(( read_size = recv(sock, client_message, 2000, 0)) > 0)
	{
		message = "You said: ";
		write(sock, message, strlen(message));
		write(sock, client_message, strlen(client_message));
	}

	if(read_size == 0)
	{
		puts("Client disconnected");
		fflush(stdout);
	}
	else if(read_size == -1)
	{
		perror("recv failed");
	}

	free(socket_desc);

	return 0;

}
