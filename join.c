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

struct list_details {
	char *node_name;
	char *node_ip;
	int node_uport;
	int node_tport;
};

void
join_tree(int uport,int tport,int r_uport,int r_tport,char* host,int k) {

	//TODO:Resolve the host name

	char *udp_buffer;
	struct sockaddr_in server_addr;
	int server_addr_size,bytes_received,bytes_sent;
	struct hostent* h;
	struct list_details l_details[20];
	//Creates a temp UDP socket
	int temp_udp_sockfd,t_index=0,i=0,s_index=0;
	int port=r_uport;
	char *list[k],*temp_str;

	if ((temp_udp_sockfd = socket (AF_INET,SOCK_DGRAM,0)) < 0) {
		fprintf (stderr,"udp: Socket couldn't be opened\n");
		exit (EXIT_FAILURE);
	}

	/* create hostent structure from  user entered host name*/
	if ( (h = gethostbyname(host)) == NULL) {
		printf("\n%s: error in gethostbyname()", "tutor");
		exit(0);
	}

	while (1) {
		udp_buffer = (char *) malloc(STRLEN);

		bzero (&server_addr,sizeof(server_addr));

		server_addr.sin_family = AF_INET;
		if (s_index == 0) {
			server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
			server_addr.sin_port = htons((size_t)port);
		}
		else
		{
			server_addr.sin_addr.s_addr = inet_addr(l_details[i].node_ip);
			server_addr.sin_port = htons((size_t)l_details[i].node_uport);
			i++;
		}
		//memcpy((char *) &server_addr.sin_addr.s_addr, h->h_addr_list[0], h->h_length);


		server_addr_size = sizeof(server_addr);

		if ((bytes_sent = sendto(temp_udp_sockfd,"join",strlen("join"),0,(struct sockaddr *)&server_addr,server_addr_size)) < 0) {
			exit (EXIT_FAILURE);
		}

		if ((bytes_received = recvfrom(temp_udp_sockfd,udp_buffer,STRLEN,0,(struct sockaddr *)&server_addr,&server_addr_size)) < 0) {
			exit (EXIT_FAILURE);
		}

		if (strcmp (udp_buffer, "accept") == 0) {
			//TODO: Open Persistent UDP Connnection
			create_udp(uport,k);
			DBG (("%s\n","Will open a TCP Connection.. Talmeee!!"));
			break;
		}
		else {
			//TODO: Read list till 'k'
			t_index = 0;
			DBG(("%s\n",udp_buffer));

			temp_str = strtok ( udp_buffer , ",");
			while (temp_str != NULL) {
				list[t_index++] = temp_str;
				temp_str = strtok (NULL, ",");
			}

			for(t_index=0;t_index<k;t_index++)
			{
				temp_str = list[t_index];
				l_details[s_index].node_name = strtok (temp_str, ":");
				l_details[s_index].node_ip = strtok (NULL, ":");
				l_details[s_index].node_uport = atoi (strtok (NULL, ":"));
				l_details[s_index].node_tport = atoi (strtok (NULL, ":"));
				s_index++;
			}
		}
		udp_buffer = NULL;
	}
	close(temp_udp_sockfd);
}
