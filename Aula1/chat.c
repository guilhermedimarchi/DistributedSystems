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
#include <sys/syscall.h>


#define BUF 40
#define PORTA 9999
#define IP "172.16.8.21"


void *tf_receive(void *args)
{
	
	int meusocket;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	int numbytes;
	socklen_t addr_size;
	char buffer[BUF];

	while(1)
	{
			meusocket = socket(AF_INET, SOCK_DGRAM, 0);

		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(PORTA);
		server_addr.sin_addr.s_addr = inet_addr(IP);
		bzero(&(server_addr.sin_zero),8);

		bzero(buffer, strlen(buffer));

		bind(meusocket, (struct sockaddr *) &server_addr, sizeof( struct sockaddr));

		addr_size = sizeof(struct sockaddr);
		
		printf("Aguardando mensagens atraves do IP: %s / Porta: %d \n", IP, PORTA);
		numbytes = recvfrom(meusocket,buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &addr_size);
		
		printf("recebendo pacotes de %s \n", inet_ntoa(client_addr.sin_addr));
		printf("o pacote tem %i bytes \n", numbytes);

		buffer[numbytes] = '\0';
		printf("MSG: %s \n", buffer);
		close(meusocket);
		
	}
	pthread_exit(NULL);
}

void *tf_send(void *args)
{
	int meusocket;
	struct sockaddr_in server_addr;
	int numbytes, porta;
	char msg[BUF], ip[16];

	printf("Digite o IP de destino \n");
	scanf("%s", ip);

	printf("Digite a porta de destino \n");
	scanf("%d", &porta);

	while(1){
		meusocket = socket (AF_INET, SOCK_DGRAM, 0);

		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(porta);
		server_addr.sin_addr.s_addr = inet_addr(ip);
		bzero(&(server_addr.sin_zero), 8);

		printf("Digite a mensagem para o host remoto \n");
		__fpurge(stdin);
		gets(msg);

		numbytes = sendto(meusocket, msg, strlen(msg), 0, (struct sockaddr *) &server_addr, sizeof(struct sockaddr));

		printf("enviado %d bytes para %s \n", numbytes, inet_ntoa(server_addr.sin_addr));
		

		close(meusocket);
	}
	pthread_exit(NULL);
}




int main(int argc, char *argv[])
{
	pthread_t t_receive, t_send;
	int i;
	void *ret;
	void *resp;
	
	ret = pthread_create(&t_receive, NULL,&tf_receive, (void*)i);
	
	resp = pthread_create(&t_send, NULL,&tf_send, (void*)i);
	
	pthread_join(t_receive, &ret);
	pthread_join(t_send, &resp);
	

}
