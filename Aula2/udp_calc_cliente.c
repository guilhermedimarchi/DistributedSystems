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
#define PORTACLIENT 9998
#define PORTASRV 9999

int main(int argc, char *argv[])
{
	int meusocket, numbytes, n1, n2,op;
	struct sockaddr_in local_addr;
	struct sockaddr_in server_addr;
	socklen_t addr_size;
	char buffer[BUF], ip[] = "127.0.0.1";

	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(PORTACLIENT);
	local_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(local_addr.sin_zero),8);

	meusocket = socket(AF_INET, SOCK_DGRAM, 0);
	bind(meusocket, (struct sockaddr *) &local_addr, sizeof( struct sockaddr));

	while(1){

		//Envia mensagem
	//	printf("Digite o IP do servidor de Calculo \n");
		//scanf("%s", ip);

		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(PORTASRV);
		server_addr.sin_addr.s_addr = inet_addr(ip);
		bzero(&(server_addr.sin_zero), 8);


		printf("Digite o primeiro valor \n");
		scanf("%i", &n1);

		printf("Digite o segundo valor \n");
		scanf("%i", &n2);

		printf("1 - Soma\n2 - Subtracao\n3 - Multiplicacao\n4 - Divisao\n \n");
		scanf("%i", &op);


		sprintf(buffer,"%i;%i;%i", n1,n2,op);
		printf("msg %s \n", buffer);

		numbytes = sendto(meusocket, buffer, strlen(buffer), 0, (struct sockaddr *) &server_addr, sizeof(struct sockaddr));

		printf("enviado %d bytes para %s \n", numbytes, inet_ntoa(server_addr.sin_addr));
		
		//Recebe mensagem
		bzero(buffer, strlen(buffer));
		addr_size = sizeof(struct sockaddr);
		
		numbytes = recvfrom(meusocket,buffer, sizeof(buffer), 0, (struct sockaddr *)&server_addr, &addr_size);
		
		buffer[numbytes] = '\0';
		printf("Resposta do servidor %s: %s \n", inet_ntoa(server_addr.sin_addr), buffer);
	}
	
	close(meusocket);
	exit (0);
}
