#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define PORTA 9999
#define IP "127.0.0.1"
#define BUF 500

int main (void)
{
	char buffer[BUF];
	int meusocket, minhaconexao;
	struct sockaddr_in local_addr;
	struct sockaddr_in conexao_addr;
	socklen_t tamanho;
	tamanho = sizeof(struct sockaddr_in);

	local_addr.sin_family = AF_INET;
	local_addr.sin_addr.s_addr = inet_addr(IP);
	local_addr.sin_port = htons(PORTA);
	bzero(&(local_addr.sin_zero), 8);
	
	meusocket = socket(AF_INET, SOCK_STREAM, 0);
		
	bind(meusocket, (struct sockaddr *)&local_addr, sizeof(struct sockaddr));
	
	listen(meusocket, 1);

	printf("TCP receiver - escutando no ip: %s porta %d tcp\n", IP, PORTA);
	
	memset(buffer, 0, sizeof(buffer));

	minhaconexao = accept(meusocket, (struct sockaddr *)&conexao_addr, &tamanho);
	printf("Uma conexao do end %s foi estabelecida! \n", inet_ntoa(conexao_addr.sin_addr));
	
	while(1)
	{
		
		bzero(buffer, BUF);
		recv(minhaconexao, buffer, 500, 0);
		printf(" msg: %s \n", buffer);
	}
	close(minhaconexao);


	close(meusocket);
	exit(0);

}
