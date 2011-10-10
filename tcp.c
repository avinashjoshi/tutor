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

int sock;            /* The socket file descriptor for our "listening"
						socket */
int connectlist[5];  /* Array of connected sockets so we know who
						we are talking to */
fd_set socks;        /* Socket file descriptors we want to wake
						up for, using select() */
int highsock;	     /* Highest #'d file descriptor, needed for select() */

void
setnonblocking(	int sock) {
	int opts;

	opts = fcntl(sock,F_GETFL);
	if (opts < 0) {
		perror("fcntl(F_GETFL)");
		exit(EXIT_FAILURE);
	}
	opts = (opts | O_NONBLOCK);
	if (fcntl(sock,F_SETFL,opts) < 0) {
		perror("fcntl(F_SETFL)");
		exit(EXIT_FAILURE);
	}
	return;
}

void
build_select_list() {
	int listnum;	     /* Current item in connectlist for for loops */

	FD_ZERO(&socks);

	FD_SET(sock,&socks);

	for (listnum = 0; listnum < 5; listnum++) {
		if (connectlist[listnum] != 0) {
			FD_SET(connectlist[listnum],&socks);
			if (connectlist[listnum] > highsock)
				highsock = connectlist[listnum];
		}
	}
}

void
handle_new_connection() {
	int listnum;	     /* Current item in connectlist for for loops */
	int connection; /* Socket file descriptor for incoming connections */

	connection = accept(sock, NULL, NULL);
	if (connection < 0) {
		DBG(("%d:%s",connection,"NOT ESTABLISHED"));
		exit(EXIT_FAILURE);
	}

	setnonblocking(connection);

	for (listnum = 0; (listnum < 5) && (connection != -1); listnum ++)
		if (connectlist[listnum] == 0) {
			connectlist[listnum] = connection;
			connection = -1;
		}
}

void 
deal_with_data(int listnum) {
	char buffer[STRLEN];     /* Buffer for socket reads */
	char buff_send[STRLEN];
	char *cur_char;      /* Used in processing buffer */
	int rbytes;
	char temp_buf[5];
	bzero(&buffer,sizeof(buffer));
	if ((rbytes = recv(connectlist[listnum],buffer,STRLEN,0)) < 0) {
		close(connectlist[listnum]);
		connectlist[listnum] = 0;
	} else if (rbytes > 0) {
		rbytes=0;
		DBG (("\nReceived: %s ",buffer));
		if (strcmp (buffer, "compute") == 0) {
			if (strcmp (parent.ip, "NULL") == 0) {
				send(connectlist[listnum],"0",4,0);
				DBG (("Sent :: 0"));
			} else {
				send (parent.established_socket, "compute", strlen("compute"), 0);
				recv (parent.established_socket, buff_send, STRLEN, 0);
				DBG (("Received :: %s", buff_send));
				sprintf (temp_buf, "%d", node_number);
				strcat (buff_send, ":");
				strcat (buff_send, temp_buf);
				send(connectlist[listnum], buff_send, strlen(buffer), 0);
				DBG (("Sent :: %s", buff_send));
			}
		}
	}
}

void
read_socks() {
	int listnum;	     /* Current item in connectlist for for loops */

	if (FD_ISSET(sock,&socks))
		handle_new_connection();

	for (listnum = 0; listnum < 5; listnum++) {
		if (FD_ISSET(connectlist[listnum],&socks))
			deal_with_data(listnum);
	}
}


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
	parent.established_socket = clientSockid;

	struct sockaddr_in servaddr;

	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons((size_t)(t_det.tport));
	servaddr.sin_addr.s_addr = inet_addr(t_det.thost);
	//inet_pton(AF_INET,t_det.thost,servaddr.sin_addr);

	int cstatus = connect(clientSockid,(struct sockaddr *)&servaddr,sizeof(servaddr));

	if (cstatus < 0) {
		fprintf (stdout, "Error in connect()");
		return;
	}

	DBG (("Persistant connection with %s:%d", t_det.thost, t_det.tport));

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

	struct timeval timeout;  /* Timeout for select */
	int readsocks;	     /* Number of sockets ready for reading */
	int reuse_addr = 1;

	sock = socket(AF_INET,SOCK_STREAM,0);

	/* So that we can re-bind to it without TIME_WAIT problems */
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse_addr,
			sizeof(reuse_addr));

	/* Set socket to non-blocking with our setnonblocking routine */
	setnonblocking(sock);

	struct sockaddr_in servaddr,clientaddr;

	//Server Address has three feilds: Family, Port and Internet Address
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons((size_t)tport);

	int bstatus = bind(sock, (struct sockaddr *)&servaddr,sizeof(servaddr));

	//Convert the socket to listening socket
	int lstatus = listen(sock,5);

	highsock = sock;
	memset((char *) &connectlist, 0, sizeof(connectlist));

	//Run an infinite loop that continuously accepts all the connection requests
	while(1)
	{
		build_select_list();
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;

		readsocks = select(highsock+1, &socks, (fd_set *) 0,(fd_set *) 0, &timeout);

		if (readsocks == 0) {
			/* Nothing ready to read, just show that
			   we're alive */
			//printf(".");
			fflush(stdout);
		} else
			read_socks();
	}
	return NULL;
}	
