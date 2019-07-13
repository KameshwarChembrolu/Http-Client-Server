/* HTTP server */
/* Run the HTTP server by entering the command line arguments "./http_server 1027 */



#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<error.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<pthread.h>



void *connection_handler(void *); 

int main(int argc, char *argv[])
	
	{
	char *PORT;
	PORT = argv[1];	

	FILE *html_data;
	html_data =  fopen("io.html","r");

	char response_data[1024];
	fgets(response_data,1024, html_data);

	char http_header[2048] = "HTTP/1.1 200 OK\r\n\n";
	char http_header1[2048] = "HTTP/1.1 200 OK File Created\r\n\n";
	
	strcat(http_header,response_data);

	int server_socket = socket(AF_INET, SOCK_STREAM, 0);
	
	struct sockaddr_in client;
	struct sockaddr_in server_address;
	int size = sizeof(struct sockaddr_in);
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(atoi(argv[1]));
	server_address.sin_addr.s_addr= INADDR_ANY;
	

 	bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));

	listen(server_socket, 8);

	puts("Waiting for the incoming connections ...");
	
	int client_socket;
	char response[1000] = {0};
	
	int len = sizeof(server_address);
	pthread_t thread_id;

	while(1)
	{

	client_socket = accept(server_socket, (struct sockadr *)&client,&len);
	
	int receive = recv(client_socket,&response, sizeof(response),0);

	if(pthread_create(&thread_id, NULL, connection_handler, (void*) &client_socket) <0)
	{
		perror("could not create thread");
		exit(-1);
	}

	puts("Handler assigned");


	if(response[0] == 'G')
	{
	printf("Recieved %s\n",response);
	send(client_socket, http_header, sizeof(http_header), 0);
	printf("Sent the file to the client\n");
	}

	else if(response[0] == 'P')
	{
	send(client_socket, http_header1, sizeof(http_header1), 0);
	char *sep = response;
	char *sep2;
	char *sep3;
	
	char *token;
	token = strtok_r(sep,"\r\n\r\n",&sep2);
	token = strtok_r(sep2,"\n",&sep3); 
		
	printf("Received the file 'index.html' from the client and it reads as follows %s\n",response);
	
	FILE *fp1;
	fp1 = fopen("hi.html","w+");
	fputs(sep3,fp1);
	fclose(fp1);
	printf("The recieved file has been successfully stored in 'hi.html'\n");
	}

	else	
	{
	char http_h2[204] = "HTTP/1.1 404 NOT FOUND\r\n\n";
	printf("Recieved %s from client \n",response);	
	send(client_socket,http_h2, sizeof(http_h2), 0);
	}	
	

	close(client_socket);
	
	}

	return 0;
}
		
	
	void *connection_handler(void *server_socket)
	{
	
	}
