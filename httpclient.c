/* HTTP Client */
/* Run the HTTP client by entering the command line arguments "./httpclient 127.0.0.1 1027 GET io.html" (or) "./httpclient 127.0.0.1 1027 PUT index.html" */


#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<errno.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h> 

int main(int argc, char *argv[])
{
	int i;
	char *address;
	address = argv[1];

	char *command;
	command = argv[3];

	char *filename;
	filename = argv[4];

	if(argc < 5)
	{
		printf("Cant proceed until you enter the required command line arguments");
		return 0;
	}

	int client_fd = socket(AF_INET,SOCK_STREAM, 0);
	
	struct sockaddr_in remote_addr;
	remote_addr.sin_family = AF_INET;
	remote_addr.sin_port = htons(atoi(argv[2]));
	inet_aton(address, &remote_addr.sin_addr.s_addr);
	
	connect(client_fd, (struct sockaddr *) &remote_addr, sizeof(remote_addr));

	char response[1000];
	char  request[1000] = "GET HTTP/1.1 \r\n\r\n";


	if((strcmp(command,"GET") == 0) && (strcmp(filename,"io.html") == 0))
	{
	
	printf("Sent GET request to the Server\n\n");
	
	
	send(client_fd, request, sizeof(request), 0);
	recv(client_fd, &response, sizeof(response), 0);

	printf("The response from the server is : %s\n", response);
	}

	
	else if((strcmp(command,"PUT") == 0) && (strcmp(filename,"index.html") == 0))
	{
	strcpy(request,"PUT HTTP/1.1 \r\n\r\n");
	printf("Sent PUT request to the server\n\n");
	FILE *fp;
	char str[60];
	
	fp = fopen("index.html", "w+");	
	fgets(str, 60, fp);
	strcat(request,str);
	send(client_fd, request, sizeof(request), 0);
	recv(client_fd, &response, sizeof(response), 0);
	printf("The response from the server : %s\n", response);
	}

	
	else 
	{
	strcpy(request,"INVALID HTTP/1.1 \r\n\r\n");
	send(client_fd, request, sizeof(request), 0);
	recv(client_fd, &response, sizeof(response), 0);
	printf("The response from the server : %s\n",response);
	}
        sleep(3);
	
	close(client_fd);
	printf("Client Disconnected\n");

}
