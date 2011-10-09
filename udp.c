/*
 * udp.c: This class listens/creates a UDP socket to accept JOIN request
 * This file is a part of Tutor
 *
 * Tutor is an application to create a tutor relationship tree using the client-server model.
 * Consists of a tutor node (server) and many student nodes (clients).
 * This was created as a part of CS 6390 Advanced Computer Networks course at UT Dallas.
 *
 * Contributors: Avinash Joshi <axj107420@utdallas.edu>, Sandeep Shenoy <sxs115220@utdallas.edu>
 */
#include "myheader.h"

int child_nodes=0;

static void* handle_udp(void*);

/*
 * This function creates a new thread for the UDP socket
 *
 */
void
create_udp (size_t uport) {

	pthread_t udp_pid;

	pthread_create(&udp_pid,NULL,&handle_udp,(void *)uport);
	return;
}

/*
 * This function opens and binds the UDP socket at the requested port
 *
 */
static void*
handle_udp (void* uport) {

	pthread_detach(pthread_self());
	int udp_sockfd,bytes_received,bytes_sent,client_addr_size;
	struct sockaddr_in server_addr,client_addr;
	char udp_buffer[STRLEN];

	//DBG (("%s \n","Creating UDP Socket ..."));

	//Creates a UDP socket
	if ((udp_sockfd = socket (AF_INET,SOCK_DGRAM,0)) < 0) {
		fprintf (stderr,"udp: Socket couldn't be opened\n");
		exit (EXIT_FAILURE);
	}

	//Creates a local server structure
	bzero (&server_addr,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons ((size_t)uport);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	/*Binds the socket*/
	if (bind (udp_sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
		fprintf (stderr,"udp: Socket couldn't be bind\n");
		exit (EXIT_FAILURE);
	}

	//DBG (("%s \n","UDP Socket created"));
	/*Blocking Receive call that keeps waiting for join request*/
	client_addr_size = sizeof(client_addr); 
	while (1)
	{
		if ((bytes_received = recvfrom(udp_sockfd,udp_buffer,STRLEN,0,(struct sockaddr *)&client_addr,&client_addr_size)) < 0) {
			exit(EXIT_FAILURE);
		}
		printf("%s\n",udp_buffer);

		//TODO: Check for k value before accepting the request
		if (strcmp( udp_buffer , "join") == 0) {
			if (child_nodes < 2 ) {
				if ((bytes_sent = sendto(udp_sockfd,"accept",strlen("accept"),0,(struct sockaddr *)&client_addr,client_addr_size)) < 0) {
					exit(EXIT_FAILURE);
				}
				child_nodes++;
			}
			else {	
				if ((bytes_sent = sendto(udp_sockfd,"S1:192.168.2.3:5678,S2:192.168.2.3:5679",strlen("S1:192.168.2.3:5678,S2:192.168.2.3:5679"),0,(struct sockaddr *)&client_addr,client_addr_size)) < 0) {
					exit(EXIT_FAILURE);
				}
			}
		}

	}

	return NULL;
}
