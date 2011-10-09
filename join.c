/*
 * join.c: Sends the UDP message containing JOIN request, on acceptance creates a persistance connection with the accepted node.
 * This file is a part of Tutor
 *
 * Tutor is an application to create a tutor relationship tree using the client-server model.
 * Consists of a tutor node (server) and many student nodes (clients).
 * This was created as a part of CS 6390 Advanced Computer Networks course at UT Dallas.
 *
 * Contributors: Avinash Joshi <axj107420@utdallas.edu>, Sandeep Shenoy <sxs115220@utdallas.edu>
 */

#include "myheader.h"

/*
 * This function requests for joining the Tutor Tree, when accpeted will create a persistance TCP connection 
 * with the node that accepted the request
 *
 */
void
join_tree(int uport,int tport,int r_uport,int r_tport,char* host) {

	//TODO:Resolve the host name

	char udp_buffer[STRLEN];
	struct sockaddr_in server_addr,client_addr;
	struct hostent *h;
	int server_addr_size,client_addr_size,bytes_received,bytes_sent;

	//Creates a temp UDP socket
	int temp_udp_sockfd;
	if ((temp_udp_sockfd = socket (AF_INET,SOCK_DGRAM,0)) < 0) {
		fprintf (stderr,"udp: Socket couldn't be opened\n");
		exit (EXIT_FAILURE);
	}

	/* create hostent structure from  user entered host name*/
	if ( (h = gethostbyname(host)) == NULL) {
		printf("\n%s: error in gethostbyname()", "tutor");
		exit(0);
	}

	//Creates a local server structure
	bzero (&server_addr,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons ((size_t)r_uport);
	//server_addr.sin_addr.s_addr = htonl(
	memcpy((char *) &server_addr.sin_addr.s_addr, h->h_addr_list[0], h->h_length);

	if ((bytes_sent = sendto(temp_udp_sockfd,"join",strlen("join"),0,(struct sockaddr *)&server_addr,server_addr_size)) < 0) {
		printf("sent??: %d",bytes_sent);
		exit(EXIT_FAILURE);
	}

	if ((bytes_received = recvfrom(temp_udp_sockfd,udp_buffer,STRLEN,0,(struct sockaddr *)&client_addr,&client_addr_size)) < 0) {
		printf("nothing: %d",bytes_received);
		exit(EXIT_FAILURE);
	}
	printf("Didn't receive anything");
	printf("%s\n",udp_buffer);

	close(temp_udp_sockfd);

}
