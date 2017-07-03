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
#define PORTACLIENT 9998
#define BUF 10

typedef struct {
	int sock;
	char ip[16];
	char str_calc[10];
} con_t; 

void * f_calc (void * arg)
{
	con_t *conexao = (con_t *) arg;
	struct sockaddr_in client_addr;
	int numbytes, n1, n2;
	char str_n1[5], str_n2[5];
	char result[5];

	printf("(IP: %s) Inf para calculo %s \n", conexao->ip, conexao->str_calc);

	strncpy(str_n1, conexao->str_calc, 1);
	strncpy(str_n2, conexao->str_calc+2, 1);

	n1 = atoi(str_n1);
	n2 = atoi(str_n2);
	
	sprintf(result,"%i", n1+n2);
	printf("Resultado %s \n", result);

	//sleep (rand() %10);

	client_addr.sin_family = AF_INET;
	client_addr.sin_port = htons(PORTACLIENT);
	client_addr.sin_addr.s_addr = inet_addr(conexao->ip);
	bzero(&(client_addr.sin_zero), 8);

	numbytes = sendto(conexao->sock, result, strlen(result), 0, (struct sockaddr *) &client_addr, sizeof(struct sockaddr));

	printf("Enviado resposta para o cliente %s (bytes %d)\n", conexao->ip, numbytes);
	pthread_exit(NULL);
}


int main (int argc, char *argv[])
{
	int meusocket, numbytes;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	pthread_t calc;
	con_t *conexao = malloc(sizeof(* conexao));
	socklen_t addr_size;
	char buffer[10];

	meusocket = socket(AF_INET, SOCK_DGRAM, 0);
	conexao->sock = meusocket;

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORTASRV);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(server_addr.sin_zero),8);

	bzero(buffer, strlen(buffer));

	bind(meusocket, (struct sockaddr *) &server_addr, sizeof( struct sockaddr));

	addr_size = sizeof(struct sockaddr);
	
	printf("Aguardando informacaoes para calculo atraves da porta: %d \n", PORTASRV);
	numbytes = recvfrom(meusocket,buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &addr_size);
	buffer[numbytes] = '\0';
	
	strcpy(conexao->str_calc, buffer);
	strcpy(conexao->ip, inet_ntoa(client_addr.sin_addr));
	
	pthread_create(&calc, NULL, &f_calc, conexao);
	pthread_join (calc, NULL);

	close(meusocket);
	
	exit(EXIT_SUCCESS);
}
