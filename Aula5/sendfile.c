#include <stdlib.h>
#include <stdio.h>
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

#define PORT 9999
#define LENGTH 1024


int main(int argc, char *argv[])
{
	int meusocket, fd;
	int data_block; 
	struct sockaddr_in remote_addr;
	char buffer[LENGTH], ip[16];
	char *caminho = malloc (50);
	
	printf("Digite o IP do servidor de arquivos \n");
	scanf("%s", ip);
	
	meusocket = socket(AF_INET, SOCK_STREAM, 0);
		
	remote_addr.sin_family = AF_INET; 
	remote_addr.sin_port = htons(PORT); 
	bzero(&(remote_addr.sin_zero), 8);

	printf("Digite o nome do arquivo que deseja copiar \n");
	__fpurge(stdin);
	gets(caminho);
	
	fd = open(caminho, O_RDONLY);

	connect(meusocket, (struct sockaddr *)&remote_addr, sizeof(struct sockaddr));

	printf("[Client] Enviando %s para o servidor... \n", caminho);

	bzero(buffer, LENGTH);
	
	do
	{
		data_block = read(fd, buffer, sizeof(buffer));
		printf("Numero de bytes enviados %i \n", data_block);
		
		if((send(meusocket, buffer, data_block, 0)) < 0)
	    {
			printf("[Client ERR] Falha no envio do arquivo %s\n", caminho);
	        exit(EXIT_FAILURE);
	    }
	    
	    if(data_block < LENGTH)
	    {
			break;
		}
	    
	    bzero(buffer, LENGTH);
	} while(data_block > 0);
	
	printf("[Client] Arquivo %s enviado com sucesso!\n", caminho);

	close (meusocket);
	printf("[Client] Conexao fechada\n");
	
	exit(EXIT_SUCCESS);
}
