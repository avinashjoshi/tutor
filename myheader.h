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
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <getopt.h>
#include <unistd.h>

#ifdef  _DEBUG_
#define DBG(x)  printf x
#else
#define DBG(x)  /* Nothing here */
#endif

#ifndef PATH_SEPARATOR
#define PATH_SEPARATOR '/'
#endif
