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
const char *com_list[] = {"help", "makeserver", "join", "computepath", "NULL"};

/* used for mapping the long options to short options */
const struct option long_options[] = {
	{ "tport", required_argument, 0, 'p' },
	{ "uport", required_argument, 0, 'P' },
	{ "help", no_argument, 0, 'h' },
	{ 0, 0, 0, 0 }
};

/* Print the usage message */
static void
print_usage (void) {
	printf ("usage: %s -p tcp_port -P udp_port [-h]\n", exec_name);
}

/* Print help message with all options */
static void
print_help (void) {
	printf ("%s is an application to create tutor-student relationship tree\n", exec_name);
	print_usage ();
	printf ("\
			-p, --tport <number>   port number to be used with TCP\n\
			-P, --uport <number>   port number to be used with UDP\n\
			-h, --help             print this help\n");
}

/*
 * This function checks if the string inputed
 * is a number or not.
 * Returns 1 if its a number else returns 0
 */
int
is_number (char * string) {
	int temp = 0;
	int is_num = 1;
	while(string[temp] != 0) {
		if(string[temp] < 48 || string[temp] > 57) is_num = 0;
		temp++;  
	}     
	return is_num;   
}

int
check_value (char *arg, char *opt) {
	int num, n = 0;
	if (strcmp ("k", opt) == 0)
		n = 2;
	if (strcmp ("port", opt) == 0)
		n = 0;

	if (!is_number (arg)) {
		fprintf (stdout, "%s must be a number", arg);
		return -1;
	}
	if ((num = atoi (arg)) < n) {
		fprintf (stdout, "%s must be greater than %d", opt, n);
		return -1;
	}
	return num;
}

/*
 * atoint_cli = argument to number
 * Function takes string as argument and returns
 * a port number if the string has a "valid" number
 * else exits the program with an FAILURE
 */
int
atoint_cli (char *arg, char opt) {
	int num;
	num = atoi(arg);
	if (!is_number (arg) || num < 0) {
		fprintf (stderr, "%s: %s: invalid number for argument -%c\n", exec_name, arg, opt);
		print_usage ();
		exit (EXIT_FAILURE);
	}
	return num;
}

int
is_command (char *com) {
	int i = 0;
	while (strcmp ("NULL", com_list[i]) != 0) {
		if (strcmp (com, com_list[i]) == 0)
			return 1;
		i++;
	}
	return 0;
}

void
list_commands (void) {
	int i = 0;
	while (strcmp ("NULL", com_list[i]) != 0) {
		printf ("%s ", com_list[i]);
		i++;
	}
	printf ("\n");
}

int
main (int argc, char **argv) {

	int optchar, opt_index = 0;
	int pflag = 0, Pflag = 0;
	int tport, uport, k;
	char *tport_arg, *uport_arg;
	char *in_line = (char *) malloc (sizeof (char) * 100);
	char *command, *com_arg, *host;
	int join_uport, join_tport;

	/* strrchr gives the last occurance of PATH_SEPARATOR in argv[0] */
	exec_name = strrchr (argv[0], PATH_SEPARATOR);
	if (!exec_name)
		exec_name = argv[0];
	else
		++exec_name;

	/* Program must have atleast one argument */
	if (argc < 2) {
		print_usage ();
		exit (EXIT_SUCCESS);
	}

	while ((optchar = getopt_long (argc, argv, "hp:P:", long_options, &opt_index)) != -1) {
		switch (optchar) {
			case 'p':
				pflag = 1;
				tport_arg = optarg;
				break;
			case 'P':
				Pflag = 1;
				uport_arg = optarg;
				break;
			case 'h':
				print_help ();
				return (EXIT_SUCCESS);
			case '?':
			default:
				print_usage ();
				return (EXIT_SUCCESS);
		}
	}

	if (pflag == 0 || Pflag == 0) {
		fprintf (stderr, "%s: -p and -P arguments are needed\n", exec_name);
		print_usage ();
		exit (EXIT_FAILURE);
	}

	tport = atoint_cli (tport_arg, 'p');
	uport = atoint_cli (uport_arg, 'P');
	node_number = 0;
	next_node = 0;

	bzero (in_line, 100);

	DBG (("TCP Port: %d\t UDP Port: %d", tport, uport));
	while (1) {
		printf("\ntutor> ");
		fflush (stdin);
		fgets (in_line, 100, stdin);
		if (strcmp (in_line, "\n") == 0)
			continue;
		in_line [strlen (in_line) - 1] = '\0';

		command = strtok (in_line, " ");

		// Capture CTLR-D and exit
		if (feof (stdin) || strcmp (command, "exit") == 0) {
			exit (EXIT_SUCCESS);
		}

		// If command does not exist in the list
		if (is_command (command) == 0) {
			fprintf (stderr, "command %s does not exist. You may want to try help\n", command);
			continue;
		}

		if (strcmp (command, "help") == 0) {
			list_commands ();
			continue;
		}

		if (strcmp (command, "makeserver") == 0) {
			com_arg = strtok (NULL, " ");
			if (com_arg == NULL) {
				fprintf (stdout, "makeserver takes one argument -- k value");
				continue;
			}

			if ((k = check_value (com_arg, "k")) == -1)
				continue;

			DBG (("k value = '%d'", k));

			create_udp (uport, tport, k);
			create_tcp (tport, k);

			continue;
		}

		if (strcmp (command, "join") == 0) {
			com_arg = strtok (NULL, " ");
			
			if (com_arg == NULL) {
				fprintf (stdout, "join takes two argument <ipaddress | host> <udp-port>");
				continue;
			}

			host = com_arg;

			com_arg = strtok (NULL, " ");
			
			if (com_arg == NULL) {
				fprintf (stdout, "join takes two argument <ipaddress | host> <udp-port>");
				continue;
			}

			if ((join_uport = check_value (com_arg, "port")) == -1)
				continue;

			join_tree (uport, tport, join_uport, 1234, host);
			DBG (("You have joined %s:%s with node number %d ", parent.ip, parent.tport, node_number));
		}

		if (strcmp (command, "computepath") == 0) {
			DBG (("%s:%s", parent.ip, parent.tport));
		}
	}

	return (EXIT_SUCCESS);
}
