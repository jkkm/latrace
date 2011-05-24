
#include <signal.h>
#include <unistd.h>

static int exit_flag = 0;

static void sig_term_handler(int sig)
{
	exit_flag = 1;
}

int main(int argc, char **argv)
{
	int ignore   = 0;
	int handlers = 1;;

	if (argc != 2)
		return -1;

	/*
	 * 1 - handlers
	 * 2 - no handlers
	 * 3 - ignore term/int
	 */

	switch(*argv[1]) {
	case '1': handlers = 1; break;
	case '2': handlers = 0; break;
	case '3': ignore   = 1; break;
	default:
		return -1;
	}

	if (handlers) {
		if ((signal(SIGTERM, sig_term_handler) == SIG_ERR) ||
		    (signal(SIGINT,  sig_term_handler) == SIG_ERR))
			return -1;
	}

	while(!exit_flag || ignore) {
		usleep(100);
	}

	return 0;
}
