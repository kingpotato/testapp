/*
 * Socket client program
 * HOST IP address and Port is hard coded.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUFF_SIZE   16384 //1549
#define PORT        8282
#define HOST        "192.168.0.5"

int   main( int argc, char **argv)
{
	int      sd, start, rev_num;
	struct   sockaddr_in   server_addr;
	FILE     *fp;
	char     buff[BUFF_SIZE];

	sd = socket(AF_INET, SOCK_STREAM, 0);
	
	if(sd < 0) {
		printf( "Failed to open socket\n");
		exit( 1);
	}

	memset( &server_addr, 0, sizeof( server_addr));
	server_addr.sin_family     = AF_INET;
	server_addr.sin_port       = htons(PORT);
	server_addr.sin_addr.s_addr= inet_addr(HOST);

	if(connect(sd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
		printf( "Failed to connect to server\n");
		exit( 1);
	}
	
	while(1) {
		rev_num = recv(sd, buff, BUFF_SIZE, 0);
		printf("Received data = %d bytes\n", rev_num);

		//Print received data as hex 
		for(start=0;start<rev_num;start+=1) {
			//Align as 16 hex bytes per one line
			if((start != 0) && (start%16 == 0))
				printf("\n");
			printf("%02X ", buff[start]);
		}
		printf("\n\n");

		//Save a file
		fp=fopen("Received_File", "wba");
		fwrite(buff, sizeof(buff), 1, fp);
		fclose(fp);
	}
	close(sd);
	return 0;
}
