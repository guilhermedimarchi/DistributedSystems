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



void * f_calc (int arg)
{
	int minhaconexao = arg;
	int numbytes;
	char buffer[BUF];
	char result[5];
	int n[3];
	char * pch;
	int i =0;
  
  
  while(1){
	  i=0;
  
	numbytes = recv(minhaconexao, buffer, 500, 0);
	buffer[numbytes] = '\0';
		

	  pch = strtok (buffer,";");
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
		case 4:
			sprintf(result,"%i", n[0]/n[1]);
			printf("Resultado %s \n", result);
			break;
			
		default:
			close(minhaconexao);
			pthread_exit(NULL);
				 
	 }

	send(minhaconexao, result,strlen(result),0);
	printf("Enviado resposta para o cliente \n");
	
}

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

   
	tamanho = sizeof(struct sockaddr_in);

	bind(meusocket, (struct sockaddr *) &server_addr, sizeof( struct sockaddr));
	listen(meusocket, 1);

while(1)
{				
			
				

				printf("Aguardando informacoes para calculo atraves da porta: %d \n", PORTASRV);
				minhaconexao = accept(meusocket, (struct sockaddr_in *)&conexao_addr, &tamanho);
				printf("Uma conexao do end %s foi estabelecida! \n", inet_ntoa(conexao_addr.sin_addr));	
			
					
				pthread_create(&calc, NULL, &f_calc, minhaconexao);
				//pthread_join (calc, NULL);
}
	close(meusocket);
	
	exit(EXIT_SUCCESS);
}
