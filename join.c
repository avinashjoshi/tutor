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
#include <signal.h>
#include <setjmp.h>

#define RECV_TIMEOUT 5	/* timeout in seconds */

static sigjmp_buf recv_timed_out;

/* timeout handler */
void
timeout_handler (int signum) {
	signal(SIGALRM, SIG_DFL);
	siglongjmp(recv_timed_out, 1);
}

struct list_details {
	char node_name[50];
	char node_ip[50];
	int node_uport;
	int nchild;
};

/*
 * This function requests for joining the Tutor Tree, when accpeted will create a persistance TCP connection 
 * with the node that accepted the request
 *
 */
int
join_tree(int uport,int tport,int r_uport,int r_tport,char* host) {

	char *udp_buffer;
	struct sockaddr_in server_addr;
	int server_addr_size, bytes_received, bytes_sent;
	struct hostent* he;
	struct list_details l_details[200];
	int temp_udp_sockfd, t_index=0, l_index=0, i=0, s_index=0;
	char **list, *t_list[5], *temp_str;

	if ((temp_udp_sockfd = socket (AF_INET,SOCK_DGRAM,0)) < 0) {
		perror ("socket() failed");
		return -1;
	}

	/* create hostent structure from  user entered host name*/
	if ((he = gethostbyname(host)) == NULL) {
		fprintf(stdout, "Unable to resolve host %s", host);
		return -1;
	}

	while (1) {
		udp_buffer = (char *) malloc(sizeof (char) * STRLEN);

		bzero (&server_addr,sizeof(server_addr));

		server_addr.sin_family = AF_INET;
		if (s_index == 0) {
			memcpy(&server_addr.sin_addr, he->h_addr_list[0], he->h_length);
			server_addr.sin_port = htons((size_t)r_uport);
		}
		else
		{
			server_addr.sin_addr.s_addr = inet_addr(l_details[i].node_ip);
			server_addr.sin_port = htons((size_t)l_details[i].node_uport);
			i++;
		}

		server_addr_size = sizeof(server_addr);

		char sendbuff[20] = "join:";
		char port_ch[7];
		sprintf (port_ch, "%d", uport);
		strcat (sendbuff, port_ch);

		if ((bytes_sent = sendto (temp_udp_sockfd, sendbuff, strlen(sendbuff), 0, (struct sockaddr *) &server_addr, server_addr_size)) < 0) {
			perror ("sendto() error");
			return -1;
		}

		if (sigsetjmp(recv_timed_out, 1)) {
			printf("recvfrom() timed out\n\n");
			return -1;
		}

		/* set timer and handler */
		signal(SIGALRM, timeout_handler);
		alarm(RECV_TIMEOUT);

		bytes_received = recvfrom (temp_udp_sockfd, udp_buffer, STRLEN, 0, (struct sockaddr *) &server_addr, (socklen_t *) &server_addr_size);

		alarm(0);
		signal(SIGALRM, SIG_DFL);

		if (bytes_received < 0) {
			perror ("recvfrom() error");
			return -1;
		}

		if (udp_buffer[0] == 'a') {
			temp_str = strtok ( udp_buffer , ":");
			while (temp_str != NULL) {
				t_list[l_index++] = temp_str;
				temp_str = strtok (NULL, ":");
			}

			strcpy (parent.ip, t_list[1]);
			strcpy (parent.tport, t_list[2]);
			strcpy (node_id, t_list[4]);
			node_number = atoi(node_id);

			create_udp(uport,tport,atoi(t_list[3]));
			make_persistance(t_list[1],atoi(t_list[2]));
			create_tcp(tport,atoi(t_list[3]));
			l_index=0;
			break;
		}
		else {
			t_index = 0;
			int list_len = 0;
			char temp2[STRLEN];

			strncpy (temp2, udp_buffer, STRLEN);
			temp_str = strtok ( udp_buffer , ",");
			while (temp_str != NULL) {
				list_len++;
				temp_str = strtok (NULL, ",");
			}

			temp_str = strtok ( temp2 , ",");

			list = malloc (sizeof(char) * list_len);

			list_len = 0;
			while (temp_str != NULL) {
				list[list_len++] = temp_str;
				temp_str = strtok (NULL, ",");
			}

			for(t_index=0;t_index<list_len;t_index++) {
				strcpy (l_details[s_index].node_name, strtok(list[t_index],":"));
				strcpy (l_details[s_index].node_ip, strtok (NULL, ":"));
				l_details[s_index].node_uport = atoi (strtok (NULL, ":"));
				l_details[s_index].nchild = atoi (strtok (NULL, ":"));
				s_index++;
			}
		}
		udp_buffer = NULL;
	}
	close(temp_udp_sockfd);
	return 1;
}
