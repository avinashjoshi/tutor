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

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif /* HAVE_UNISTD_H */
#include <sys/types.h>
#ifdef HAVE_STRING_H
#include <string.h>
#else
#include <strings.h>
#endif /* HAVE_STRING_H */

/* To print DEBUG printf's */
#ifdef  _DEBUG_
#define DBG(x)  printf x
#else
#define DBG(x)  /* Nothing here */
#endif

/* For strrchr function */
#ifndef PATH_SEPARATOR
#define PATH_SEPARATOR '/'
#endif
