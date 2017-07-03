#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

int main (int argc, char *argv[])
{
	int meusocket,porta;
	struct sockaddr_in server_addr;
	char str[500], ip[15];
	
	printf("TCP sender \n");
	
	printf("digite o ip do destinatario \n");
	scanf("%s",ip);
	
	printf("digite a porta do destinatario \n");
	scanf("%d", &porta);

	meusocket = socket(AF_INET, SOCK_STREAM, 0);
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(ip);
	server_addr.sin_port = htons(porta);
	bzero(&(server_addr.sin_zero), 8);

	connect(meusocket,(struct sockaddr *)&server_addr, sizeof(struct sockaddr));

	while(1)
	{
		bzero(str, 500);
		printf ("Digite a msg \n");
		
		__fpurge(stdin);
		gets(str);
		send(meusocket, str,strlen(str),0);
	}
	close(meusocket);

	exit(0);
}
