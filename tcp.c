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

static void* handle_persistance (void*);

static
struct t_details {
	char *thost;
	int tport;
} t_det;

void
make_persistance (char *t_host, int tport) {

	pthread_t t_pid;

	t_det.thost = t_host;
	t_det.tport = tport;


	pthread_create(&t_pid,NULL,&handle_persistance,(void *)10);
}	

static void*
handle_persistance (void *p) {
	/* Create a client Socket
	 *  Connect to the Server Socket
	 *  Send data
	 *  Receive data
	 */
	

	pthread_detach(pthread_self());


	int clientSockid = socket(AF_INET,SOCK_STREAM,0);

	struct sockaddr_in servaddr;

	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons((size_t)(t_det.tport));
	servaddr.sin_addr.s_addr = inet_addr((t_det.thost));
	//inet_pton(AF_INET,t_det.thost,servaddr.sin_addr);

	int cstatus = connect(clientSockid,(struct sockaddr *)&servaddr,sizeof(servaddr));
	printf("Client Connection Status : %d\n",cstatus);

	/*Send and Read data
	char sendD[STRLEN],recvD[STRLEN];

	//Read Input from Console
	printf("Enter Data: ");
	fgets(sendD,STRLEN,stdin);

	int sbyte = send(clientSockid,sendD,strlen(sendD),0);
	printf("Sent Bytes: %d\n",(int)strlen(sendD));

	int rbyte = recv(clientSockid,recvD,STRLEN,0);
	//fputs(recvD,stdout);

	while((rbyte = recv(clientSockid,recvD,STRLEN,0)) <0)
	{
		recvD[rbyte] = 0;
		fputs(recvD,stdout);
	}

	printf("Received Bytes: %d\n",(int)strlen(recvD));

	close(clientSockid);*/
}
void
create_tcp (size_t tport,int k) {

	pthread_t tcp_pid;
	pthread_create (&tcp_pid,NULL,&handle_tcp,(void *) tport);

}

static void* 
handle_tcp (void* tport) {
	/*	Create a Socket
	 *	Bind the Socket to a port
	 *	Convert the socket to a listening socket
	 *	Accept the connection as and when the request arrives
	 */
	int sockid = socket(AF_INET,SOCK_STREAM,0);

	struct sockaddr_in servaddr,clientaddr;

	//Server Address has three feilds: Family, Port and Internet Address
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons((size_t)tport);

	int bstatus = bind(sockid, (struct sockaddr *)&servaddr,sizeof(servaddr));

	//Convert the socket to listening socket
	int lstatus = listen(sockid,5);

	//Run an infinite loop that continuously accepts all the connection requests
	for(;;)
	{
		int clientlen = sizeof(clientaddr);
		size_t clientSockid = accept(sockid,(struct sockaddr *)&clientaddr,&clientlen);
	}
	return NULL;
}	
