// setting:
#include <stdio.h>
#include <sys/socket.h>

int main(int argc, char *argv[]) 
{
	int socket_desc;
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	//Address Family - AF_INET (this is IP version 4) 
	//Type - SOCK_STREAM (this means connection oriented TCP protocol) 
	//Protocol - 0 [ or IPPROTO_IP This is IP protocol]
	
	if(socket_desc == -1){
		printf("Could not create socket");
	}
	return 0;
}
