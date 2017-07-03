#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <strings.h>
#include <arpa/inet.h>
#include <pthread.h>


#define PORTASRV 9999
#define BUF 10

void * f_calc (void * arg)
{
	int minhaconexao = (int) arg;
	int numbytes, n1, n2;
	char buffer[BUF];
	char str_n1[5], str_n2[5];
	char result[5];

	numbytes = recv(minhaconexao, buffer, 500, 0);
	buffer[numbytes] = '\0';
	
	printf("Inf para calculo %s \n", buffer);

	strncpy(str_n1, buffer, 1);
	strncpy(str_n2, buffer+2, 1);

	n1 = atoi(str_n1);
	n2 = atoi(str_n2);
	
	sprintf(result,"%i", n1+n2);
	printf("Resultado %s \n", result);

	send(minhaconexao, result,strlen(result),0);
	printf("Enviado resposta para o cliente \n");
	
	
	close(minhaconexao);
	pthread_exit(NULL);
}


int main (int argc, char *argv[])
{
	int meusocket, minhaconexao;
	struct sockaddr_in server_addr;
	struct sockaddr_in conexao_addr;
	pthread_t calc;
	socklen_t tamanho;
	char buffer[10];

	meusocket = socket(AF_INET, SOCK_STREAM, 0);

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORTASRV);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(server_addr.sin_zero),8);

	bzero(buffer, strlen(buffer));

	bind(meusocket, (struct sockaddr *) &server_addr, sizeof( struct sockaddr));

	listen(meusocket, 1);

	tamanho = sizeof(struct sockaddr);
	
	printf("Aguardando informacoes para calculo atraves da porta: %d \n", PORTASRV);
	minhaconexao = accept(meusocket, (struct sockaddr *)&conexao_addr, &tamanho);
	printf("Uma conexao do end %s foi estabelecida! \n", inet_ntoa(conexao_addr.sin_addr));	
	
	pthread_create(&calc, NULL, &f_calc, (void *) minhaconexao);
	pthread_join (calc, NULL);

	close(meusocket);
	
	exit(EXIT_SUCCESS);
}