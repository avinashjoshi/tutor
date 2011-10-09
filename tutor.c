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
const char *commands[] = {"help", "makeserver", "join", "computepath", "NULL"};

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
	printf ("usage: %s -p tcp_port -P udp_port -k number [-h]\n", exec_name);
}

/* Print help message with all options */
static void
print_help (void) {
	printf ("%s is an application to create tutor-student relationship tree\n", exec_name);
	print_usage ();
	printf ("\
			-p, --tport <number>   port number to be used with TCP\n\
			-P, --uport <number>   port number to be used with UDP\n\
			-k          <number>   number of TCP connections allowed >= 2 \n\
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

/*
 * atoint = argument to number
 * Function takes string as argument and returns
 * a port number if the string has a "valid" number
 * else exits the program with an FAILURE
 */
int
atoint (char *arg, char opt) {
	int num;
	num = atoi(arg);
	if (!is_number (arg) || num < 0 || (opt == 'k' && num < 2)) {
		fprintf (stderr, "%s: %s: invalid number for argument -%c\n", exec_name, arg, opt);
		print_usage ();
		exit (EXIT_FAILURE);
	}
	return num;
}

int
is_command (char *com) {
	int i = 0;
	while (strcmp ("NULL", commands[i]) != 0) {
		if (strcmp (com, commands[i]) == 0)
			return 1;
		i++;
	}
	return 0;
}

void
list_commands (void) {
	int i = 0;
	while (strcmp ("NULL", commands[i]) != 0) {
		printf ("%s ", commands[i]);
		i++;
	}
	printf ("\n");
}

int
main (int argc, char **argv) {

	int optchar, opt_index = 0;
	int pflag = 0, Pflag = 0, kflag = 0;
	int tport, uport, k = 0;
	char *tport_arg, *uport_arg, *k_arg;

	DBG (("%s, %s", commands[0], commands[1]));

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

	while ((optchar = getopt_long (argc, argv, "hp:P:k:", long_options, &opt_index)) != -1) {
		switch (optchar) {
			case 'p':
				pflag = 1;
				tport_arg = optarg;
				break;
			case 'P':
				Pflag = 1;
				uport_arg = optarg;
				break;
			case 'k':
				kflag = 1;
				k_arg = optarg;
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

	if (pflag == 0 || Pflag == 0 || kflag == 0) {
		fprintf (stderr, "%s: -p, -P and -k arguments are needed\n", exec_name);
		print_usage ();
		exit (EXIT_FAILURE);
	}

	tport = atoint (tport_arg, 'p');
	uport = atoint (uport_arg, 'P');
	k = atoint (k_arg, 'k');

	DBG (("TCP Port: %d\t UDP Port: %d", tport, uport));
	while (1) {
		printf("tutor> ");
		scanf("%s", argv[1]);

		if (strcmp (argv[1], "help") == 0) {
			list_commands ();
			continue;
		}

		if (strcmp (argv[1], "makeserver") == 0) {
			create_udp (uport);
			continue;
		}

		if (strcmp (argv[1], "join") == 0) {
			//TODO: Accept tport and uport of root from join, for not it takes from ./tutor 
			join_tree(1235,5679,uport,tport,"192.168.2.6");
		}
		if (feof (stdin) || strcmp (argv[1], "exit") == 0) {
			exit (EXIT_SUCCESS);
		}

		if (is_command (argv[1]) == 0)
			fprintf (stderr, "command %s does not exist. You may want to try help\n", argv[1]);
	}

	return (EXIT_SUCCESS);
}
