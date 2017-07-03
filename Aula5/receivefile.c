#include <stdlib.h>
#include <stdio.h>
#include <stdio_ext.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <signal.h>
#include <ctype.h>          
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>

#define PORTA 9999
#define LENGTH 1024
#define BACKLOG 10

void *f_upload (void * arg)
{
	int minhaconexao = (int) arg;
	int fd; 
	int data_block;
	char buffer[LENGTH]; 
	char caminho[50];
	
	printf("Cliente solicitou upload de arquivo! Digite o nome do arquivo para salvar em disco. \n");
	__fpurge(stdin);
	gets(caminho);
		
	fd = open(caminho, O_WRONLY | O_CREAT, 0666);
		
	bzero(buffer, LENGTH);
	
	do
	{
		data_block = recv(minhaconexao, buffer, sizeof(buffer), 0);
	    if(data_block < 0)
		{
			perror("[Server ERR] Erro na recepcao do arquivo \n");
			break;
		}
		else if(data_block == 0)
		{
			printf("[Server] Arquivo recebido com sucesso! \n");
			break;
		}
		else
		{
			printf("Numero de bytes recebidos %i \n", data_block);
			write(fd, buffer, data_block);
		}
			
		bzero(buffer, LENGTH);
	} while (data_block > 0 );
		 
	close(minhaconexao);
	
	pthread_exit(NULL);
}


int main ()
{
	pthread_t upload;
	int meusocket, minhaconexao; 
	struct sockaddr_in addr_local;
	struct sockaddr_in addr_remote;
	socklen_t tamanho = sizeof(struct sockaddr_in);

	printf("[Server] Servidor de arquivos \n");
	
	meusocket = socket(AF_INET, SOCK_STREAM, 0);
	
	addr_local.sin_family = AF_INET; 
	addr_local.sin_port = htons(PORTA); 
	addr_local.sin_addr.s_addr = INADDR_ANY;
	bzero(&(addr_local.sin_zero), 8);

	bind(meusocket, (struct sockaddr*)&addr_local, sizeof(struct sockaddr));

	listen(meusocket, BACKLOG);
		
	while(1)
	{
		printf("[Server] Aguardando conexao \n");
	    minhaconexao = accept(meusocket, (struct sockaddr *)&addr_remote, &tamanho);

		pthread_create(&upload, NULL, &f_upload, (void *) minhaconexao);

		
	}
	
	close(meusocket);
	exit(EXIT_SUCCESS);
}
