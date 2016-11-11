#include <stdio.h>    //printf(3)
#include <stdlib.h>   //exit(3)
#include <unistd.h>   //fork(3), chdir(3), sysconf(3), setsid(2)
#include <signal.h>   //signal(3)
#include <sys/stat.h> //umask(3)
#include "log.h"


#define DEV_NULL "/dev/null"

int daemonize(const char* name, const char* path, const char* outfile, const char* errfile, const char* infile, bool fork_proc = true)
{
	if (!path) { path = "/"; }
	if (!name) { name = "medaemon"; }
	if (!infile) { infile = DEV_NULL; }
	if (!outfile) { outfile = DEV_NULL; }
	if (!errfile) { errfile = DEV_NULL; }
	//printf("%s %s %s %s\n",name,path,outfile,infile);
	pid_t child;
	if (fork_proc) {
		//fork, detach from process group leader
		if ((child = fork()) < 0) { //failed fork
			fprintf(stderr, "error: failed fork\n");
			exit(EXIT_FAILURE);
		}
		if (child > 0) { //parent
			exit(EXIT_SUCCESS);
		}

		if (setsid() < 0) { //failed to become session leader
			fprintf(stderr, "error: failed setsid\n");
			exit(EXIT_FAILURE);
		}

		//catch/ignore signals
		signal(SIGCHLD, SIG_IGN);
		signal(SIGHUP, SIG_IGN);

		//fork second time
		if ((child = fork()) < 0) { //failed fork
			fprintf(stderr, "error: failed fork\n");
			exit(EXIT_FAILURE);
		}
		if (child > 0) { //parent
			exit(EXIT_SUCCESS);
		}

		//new file permissions
		umask(0);
		//change to path directory
		chdir(path);

		//Close all open file descriptors
		int fd;
		for (fd = (int)sysconf(_SC_OPEN_MAX); fd > 0; --fd)
		{
			close(fd);
		}

	}
	//reopen stdin, stdout, stderr
	freopen(infile, "r", stdin);
	freopen(outfile, strcmp(DEV_NULL, outfile) == 0 ? "w+" : "a", stdout);
	freopen(errfile, strcmp(DEV_NULL, errfile) == 0 ? "w+" : "a", stderr);

	//open syslog
	log::open(name, LOG_PID);
	return(0);
}

#undef DEV_NULL