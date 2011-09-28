/*
 * tutor.c: Server side code handles client/student requests
 * This file is a part of Tutor
 *
 * Tutor is an application to create a tutor relationship tree using the client-server model.
 * Consists of a tutor node (server) and many student nodes (clients).
 * This was created as a part of CS 6390 Advanced Computer Networks course at UT Dallas.
 *
 * Contributors: Avinash Joshi <axj107420@utdallas.edu>, Sandeep Shenoy <sxs115220@utdallas.edu>
 */

/* Include header files */
#include "myheader.h"

const char *exec_name;

/* used for mapping the long options to short options */
const struct option long_options[] = {
    { "tport", required_argument, 0, 'p' },
    { "uport", required_argument, 0, 'P' },
    { "help", no_argument, 0, 'h' },
    { 0, 0, 0, 0 }
};

/* Print the usage message */
static void  print_usage (void) {
    printf ("Usage: %s [-p | --tport] <tcp-port> [-P | --uport] <udp-port> [-n] <clients> [-h]\n", exec_name);
}

/* Print help message with all options */
static void print_help (void) {
    printf ("%s is an application to create tutor-student relationship tree\n", exec_name);
    print_usage ();
    printf ("\
            -p, --tport <number>   port number to be used with TCP\n\
            -P, --uport <number>   port number to be used with UDP\n\
            -n          <number>   Number of students a node can handle >= 2 \n\
            -h, --help             print this help\n");
}

/*
 * This function checks if the string inputed
 * is a number or not.
 * Returns 1 if its a number else returns 0
 */
int is_number (char * string) {
    int temp = 0;
    int is_num = 1;
    while(string[temp] != 0) {
        if(string[temp] < 48 || string[temp] > 57) is_num = 0;
        temp++;  
    }     
    return is_num;   
}

int main (int argc, char **argv) {

    /* strrchr gives the last occurance of PATH_SEPARATOR in argv[0] */
    exec_name = strrchr (argv[0], PATH_SEPARATOR);
    if (!exec_name)
        exec_name = argv[0];
    else
        ++exec_name;

    int optchar, opt_index = 0;
    int pflag = 0, Pflag = 0;
    int tport, uport, nclients = 0;

    /* Program must have atleast one argument */
    if (argc < 2) {
        print_usage ();
        exit (EXIT_SUCCESS);
    }

    while ((optchar = getopt_long (argc, argv, "hp:P:n:", long_options, &opt_index)) != -1) {
        switch (optchar) {
            case 'p':
                pflag = 1;
                tport = atoi (optarg);
                if (!is_number (optarg)) {
                    fprintf (stderr, "%s: %s: argument to -p must be a number\n", exec_name, optarg);
                    exit (EXIT_FAILURE);
                }
                if (tport < 0) {
                    fprintf (stderr, "%s: %d: port number must be positive\n", exec_name, tport);
                    exit (EXIT_FAILURE);
                }
                break;
            case 'P':
                Pflag = 1;
                break;
            case 'n':
                nclients = atoi (optarg);
                if (!is_number (optarg)) {
                    fprintf (stderr, "%s: %s: argument to -n must be a number\n", exec_name, optarg);
                    exit (EXIT_FAILURE);
                }
            case 'h':
                print_help ();
                return (EXIT_SUCCESS);
            case '?':
                print_usage ();
                return (EXIT_SUCCESS);
            default:
                print_help ();
                abort();
        }
    }

    if (pflag == 0 || Pflag == 0 || nclients == 0) {
        fprintf (stderr, "%s: -p, -P and -n arguments are needed\n", exec_name);
        print_usage ();
        exit (EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
