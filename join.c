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
join_tree(int uport,int tport,int r_uport,int r_tport,char* host) {

	//TODO:Resolve the host name

	char *udp_buffer;
	struct sockaddr_in server_addr;
	int server_addr_size,bytes_received,bytes_sent;
	struct hostent* h;
	struct list_details l_details[20];
	//Creates a temp UDP socket
	int temp_udp_sockfd,t_index=0,l_index=0,i=0,s_index=0;
	char *list[2],*t_list[4],*temp_str;

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
			server_addr.sin_port = htons((size_t)r_uport);
		}
		else
		{
			server_addr.sin_addr.s_addr = inet_addr(l_details[i].node_ip);
			server_addr.sin_port = htons((size_t)l_details[i].node_uport);
			i++;
		}
		//memcpy((char *) &server_addr.sin_addr.s_addr, h->h_addr_list[0], h->h_length);


		server_addr_size = sizeof(server_addr);
		
		char sendbuff[20] = "join:";
		char port_ch[7];
		sprintf (port_ch, "%d", uport);
		strcat (sendbuff, port_ch);

		if ((bytes_sent = sendto(temp_udp_sockfd,sendbuff,strlen(sendbuff),0,(struct sockaddr *)&server_addr,server_addr_size)) < 0) {
			exit (EXIT_FAILURE);
		}

		if ((bytes_received = recvfrom(temp_udp_sockfd,udp_buffer,STRLEN,0,(struct sockaddr *)&server_addr,&server_addr_size)) < 0) {
			exit (EXIT_FAILURE);
		}


		//if (strcmp (udp_buffer, "accept") == 0) {
		if (udp_buffer[0] == 'a') {
			//TODO: Open Persistent UDP Connnection, split the udp response into tokens
			temp_str = strtok ( udp_buffer , ":");
			while (temp_str != NULL) {
				t_list[l_index++] = temp_str;
				temp_str = strtok (NULL, ":");
			}
			for(l_index=0;l_index<4;l_index++)
			{
				printf("%s -- ",t_list[l_index]);
			}
			DBG (("Accecpted by ---- %d", uport));
			create_udp(uport,tport,atoi(t_list[3]));
			make_persistance(t_list[1],atoi(t_list[2]));
			DBG (("Structure ---====> '%s' '%s' '%s' '%s'", t_list[0],t_list[1],t_list[2],t_list[3]));
			create_tcp(tport,atoi(t_list[3]));
			DBG (("%s\n","Will open a TCP Connection.. Talmeee!!"));
			l_index=0;
			break;
		}
		else {
			//TODO: Read list till 'k'
			DBG (("Third node"));
			t_index = 0;
			DBG(("%s\n",udp_buffer));

			temp_str = strtok ( udp_buffer , ",");
			while (temp_str != NULL) {
				list[t_index++] = temp_str;
				temp_str = strtok (NULL, ",");
			}

			int list_len = t_index;
			DBG (("%d", list_len));

			for(t_index=0;t_index<list_len;t_index++)
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
