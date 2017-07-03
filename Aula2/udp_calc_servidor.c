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

#define BUF 40

typedef struct {
	int sock;
	char ip[16];
	char str_calc[10];
} con_t; 

void * f_calc (void * arg)
{
	con_t *conexao = (con_t *) arg;
	struct sockaddr_in client_addr;
	int numbytes;

	char result[5];
 int n[3];
  char * pch;
  int i =0;
 
	printf("(IP: %s) Inf para calculo %s \n", conexao->ip, conexao->str_calc);

	  pch = strtok (conexao->str_calc,";");
	  while (pch != NULL)
	  {
		printf ("%s\n",pch);
		n[i] = atoi(pch);
		pch = strtok (NULL, ";");
		i++;
	  }
	  
	 switch(n[2])
	 {
		 case 1:
			sprintf(result,"%i", n[0]+n[1]);
			printf("Resultado %s \n", result);
			break;
		 case 2:
			sprintf(result,"%i", n[0]-n[1]);
			printf("Resultado %s \n", result);
			break;
			
			
		case 3:
			sprintf(result,"%i", n[0]*n[1]);
			printf("Resultado %s \n", result);
			break;
			
		default:
			sprintf(result,"%i", n[0]/n[1]);
			printf("Resultado %s \n", result);
			break;

		 
	 }
	  
	client_addr.sin_family = AF_INET;
	client_addr.sin_port = htons(PORTACLIENT);
	client_addr.sin_addr.s_addr = inet_addr(conexao->ip);
	bzero(&(client_addr.sin_zero), 8);

	numbytes = sendto(conexao->sock, result, strlen(result), 0, (struct sockaddr *) &client_addr, sizeof(struct sockaddr));

	printf("Enviado resposta para o cliente %s (bytes %d) \n", conexao->ip, numbytes);
	pthread_exit(NULL);
}


int main (int argc, char *argv[])
{
	int meusocket, numbytes;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	pthread_t calc;
	con_t *conexao = malloc(sizeof(conexao));
	socklen_t addr_size;
	char buffer[10];

	//AF_INET = Falando que vai trabalhar com IP
	//SOCK_DGRAM = Falando que vai trabalhar com UDP
	meusocket = socket(AF_INET, SOCK_DGRAM, 0);
	
	//Atribui o socket a conexao
	conexao->sock = meusocket;

	//Cria estrutura para receber (sempre responder as tres perguntas: sin_family, sin_port e sin_addr)
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORTASRV); //htons (host to network traduz porta para endereço de rede)
	server_addr.sin_addr.s_addr = INADDR_ANY; //Traduz endereço binário. INADDR_ANY informa que qualquer interface pode trazer informação
	bzero(&(server_addr.sin_zero),8); 

	//Limpa o buffer
	bzero(buffer, strlen(buffer));

	//Amarra a estrutura no socket para receber informações
	bind(meusocket, (struct sockaddr *) &server_addr, sizeof( struct sockaddr));

	addr_size = sizeof(struct sockaddr);
	
	
	while(1){
	printf("Aguardando informacaoes para calculo atraves da porta: %d \n", PORTASRV);
	numbytes = recvfrom(meusocket,buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &addr_size);
	buffer[numbytes] = '\0';
	
	strcpy(conexao->str_calc, buffer);
	strcpy(conexao->ip, inet_ntoa(client_addr.sin_addr));
	
	pthread_create(&calc, NULL, &f_calc, conexao);
	pthread_join (calc, NULL);
}
	close(meusocket);
	
	exit(EXIT_SUCCESS);
}
