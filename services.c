/*
 * services.c: Provides all services after persistant TCP connection established
 * This file is a part of Tutor
 *
 * Tutor is an application to create a tutor relationship tree using the client-server model.
 * Consists of a tutor node (server) and many student nodes (clients).
 * This was created as a part of CS 6390 Advanced Computer Networks course at UT Dallas.
 *
 * Contributors: Avinash Joshi <axj107420@utdallas.edu>, Sandeep Shenoy <sxs115220@utdallas.edu>
 */

#include "myheader.h"

void
compute_path (void) {
	char buff[STRLEN];
	char temp_buf[10];
	send (parent.established_socket, "compute", strlen("compute"), 0);
	recv (parent.established_socket, buff, STRLEN, 0);
	sprintf (temp_buf, "%d", node_number);
	strcat (buff, ":");
	strcat (buff, temp_buf);
	DBG (("Path: %s", buff));
}
