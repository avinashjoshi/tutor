/*
 * myheader.h: Contains the common function names or other header files
 * This file is a part of Tutor
 *
 * Tutor is an application to create a tutor relationship tree using the client-server model.
 * Consists of a tutor node (server) and many student nodes (clients).
 * This was created as a part of CS 6390 Advanced Computer Networks course at UT Dallas.
 *
 * Contributors: Avinash Joshi <axj107420@utdallas.edu>, Sandeep Shenoy <sxs115220@utdallas.edu>
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <pthread.h>
#include <netinet/in.h>
#include <netdb.h>

/*Buffer Size for every read and write calls*/
#define STRLEN 1024

#define _DEBUG_
/* To print DEBUG printf's */
#ifdef  _DEBUG_
#define DBG(x)  printf ("DEBUG: "); printf x; printf("\n");
#else
#define DBG(x)  /* Nothing here */
#endif

/* For strrchr function */
#ifndef PATH_SEPARATOR
#define PATH_SEPARATOR '/'
#endif

void 
create_udp (size_t,int);

void
create_tcp(size_t,char*,int);

void 
join_tree (int,int,int,int,char*);
