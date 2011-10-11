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

int child_count=0;
int k_child=0;
int tcpport=0;
static void* handle_udp(void*);

struct node_details {
	int nid;
	char node_ip[30];
	int node_udpport;
	int nchildren;
}node_det[10];
/*
 * This function creates a new thread for the UDP socket
 *
 */
void
create_udp (size_t uport,size_t tport,int k) {
	k_child = k;
	tcpport = tport;
	
	//node_det = (struct node_details *)malloc(sizeof(*node_det)*k);
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
	char udp_list_buffer[STRLEN];
	char temp_k[20];

	int m=0;

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

	bzero (&udp_list_buffer,sizeof(udp_list_buffer));

char server_host[50];
struct hostent *he;
	gethostname (server_host, 49);
	 if ((he = gethostbyname(server_host)) == NULL) {
	          herror(server_host);
	          puts("error resolving hostname..");
	          return;
	 }
	struct in_addr **tmp = (struct in_addr **) he->h_addr_list;
	DBG (("Hostname:IP = %s:%s", server_host, inet_ntoa(*tmp[0])));
	/*Binds the socket*/
	if (bind (udp_sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
		fprintf (stderr,"udp: Socket couldn't be bind\n");
		exit (EXIT_FAILURE);
	}
	int next_node = (node_number * k_child) + 1;

	//DBG (("%s \n","UDP Socket created"));
	/*Blocking Receive call that keeps waiting for join request*/
	client_addr_size = sizeof(client_addr); 
	while (1)
	{
		bzero (&udp_buffer,sizeof(udp_buffer));
		char temp[STRLEN];
		if ((bytes_received = recvfrom(udp_sockfd,udp_buffer,STRLEN,0,(struct sockaddr *)&client_addr,&client_addr_size)) < 0) {
			exit(EXIT_FAILURE);
		}

		char temp_buff[50], *temp_port, *temp_command;
		int child_port;
		strcpy(temp_buff,udp_buffer);

		temp_command = strtok (temp_buff, ":");
		temp_port = strtok (NULL, ":");
		child_port = atoi(temp_port);

		//TODO: Check for k value before accepting the request
		if (strcmp( temp_command , "join") == 0) {
			if (child_count < k_child ) {
				strcpy(temp,"accept:");
				sprintf(temp_k,"%s", inet_ntoa(*tmp[0]));
				strcat(temp,temp_k);
				node_det[child_count].nid = node_number;
				strcpy(node_det[child_count].node_ip,inet_ntoa(client_addr.sin_addr));
				strcat(temp,":");
				sprintf(temp_k,"%d",tcpport);
				node_det[child_count].node_udpport = child_port;
				strcat(temp, temp_k);
				strcat(temp,":");
				sprintf(temp_k,"%d",k_child);
				node_det[child_count].nchildren = k_child;
				strcat(temp, temp_k);
				strcat(temp,":");
				sprintf(temp_k,"%d",next_node);
				strcat(temp, temp_k);
				strcat(temp,"\0");
				next_node++;
				DBG (("Sending %s", temp));
				if ((bytes_sent = sendto(udp_sockfd,temp,strlen(temp),0,(struct sockaddr *)&client_addr,client_addr_size)) < 0) {
					exit(EXIT_FAILURE);
				}
				child_count++;
			}
			else {	
				char temp_b[30];
				bzero (temp, sizeof(temp));
				for (m=0;m<k_child;m++) {
					sprintf(temp_b,"%d",node_det[m].nid);
					strcat(temp,temp_b);
					strcat(temp,":");
					strcat(temp,node_det[m].node_ip);
					strcat(temp,":");
					sprintf(temp_b,"%d",node_det[m].node_udpport);
					strcat(temp,temp_b);
					strcat(temp,":");
					sprintf(temp_b,"%d",node_det[m].nchildren);
					strcat(temp,temp_b);
					if(m != k_child-1)
						strcat(temp,",");	
				}
				DBG (("Sending %s",temp));
				if ((bytes_sent = sendto(udp_sockfd,temp,strlen(temp),0,(struct sockaddr *)&client_addr,client_addr_size)) < 0) {
					exit(EXIT_FAILURE);
				}
			}
		}

	}

	return NULL;
}
