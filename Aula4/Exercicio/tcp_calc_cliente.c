#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <strings.h>
#include <arpa/inet.h>
#include <stdio_ext.h>
#include <pthread.h>

#define BUF 10
#define PORTASRV 9999

int main(int argc, char *argv[])
{
	int meusocket, numbytes, n1, n2,op;
	struct sockaddr_in server_addr;
	char buffer[BUF], ip[16];

	meusocket = socket(AF_INET, SOCK_STREAM, 0);

	printf("Digite o IP do servidor de Calculo \n");
	scanf("%s", ip);

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORTASRV);
	server_addr.sin_addr.s_addr = inet_addr(ip);
	bzero(&(server_addr.sin_zero), 8);

while(1)
{

		printf("Digite o primeiro valor \n");
		scanf("%i", &n1);

		printf("Digite o segundo valor \n");
		scanf("%i", &n2);

		printf("1 - Soma\n2 - Subtracao\n3 - Multiplicacao\n4 - Divisao\n5 - SAIR \n");
		scanf("%i", &op);

	

		sprintf(buffer,"%i;%i;%i", n1,n2,op);
		printf("msg %s \n", buffer);
		connect(meusocket,(struct sockaddr *)&server_addr, sizeof(struct sockaddr));	
		printf("enviado msg para %s \n", inet_ntoa(server_addr.sin_addr));
		send(meusocket, buffer,strlen(buffer),0);
		if(op==5)
		{
				close(meusocket);
				exit (0);
		}

	bzero(buffer, strlen(buffer));
	
	numbytes = recv(meusocket, buffer, 500, 0);
	buffer[numbytes] = '\0';
	printf("Resposta do servidor %s: %s \n", inet_ntoa(server_addr.sin_addr), buffer);
	
}
	
	close(meusocket);
	exit (0);
}
