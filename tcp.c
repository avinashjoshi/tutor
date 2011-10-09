/*
 * tcp.c: Code to create persistent TCP Connection with a tutor once client's join request is accepted.
 * This file is a part of Tutor
 *
 * Tutor is an application to create a tutor relationship tree using the client-server model.
 * Consists of a tutor node (server) and many student nodes (clients).
 * This was created as a part of CS 6390 Advanced Computer Networks course at UT Dallas.
 *
 * Contributors: Avinash Joshi <axj107420@utdallas.edu>, Sandeep Shenoy <sxs115220@utdallas.edu>
 */
#include"myheader.h"

static void* handle_tcp (void*);
char *t_host;

void
create_tcp (size_t tport,char* host,int k) {

	pthread_t tcp_pid;
	t_host=host;
	pthread_create (&tcp_pid,NULL,&handle_tcp,(void *) tport);
	return;

}

static void* 
handle_tcp (void* tport) {
	/*	Create a Socket
	 *	Bind the Socket to a port
	 *	Convert the socket to a listening socket
	 *	Accept the connection as and when the request arrives
	 */
	printf("Creating Socket ...\n");
	int sockid = socket(AF_INET,SOCK_STREAM,0);
	printf("Socket Descriptor : %d\n",sockid);

	struct sockaddr_in servaddr,clientaddr;
	int port;

	//Server Address has three feilds: Family, Port and Internet Address
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(t_host);
	servaddr.sin_port = htons((size_t)port);

	int bstatus = bind(sockid, (struct sockaddr *)&servaddr,sizeof(servaddr));
	printf("Bind Status : %d\n",bstatus);

	//Convert the socket to listening socket
	int lstatus = listen(sockid,5);
	printf("Listen Status :  %d\n",lstatus);

	//Run an infinite loop that continuously accepts all the connection requests
	for(;;)
	{
		printf("1");
		int clientlen = sizeof(clientaddr);
		size_t clientSockid = accept(sockid,(struct sockaddr *)&clientaddr,&clientlen);
		printf("Accept Status : %d\n",(int)clientSockid);
		printf("Request came from %s\n",inet_ntoa(clientaddr.sin_addr));
	}
	return NULL;
}	
