/*
 *  User space test program:
 *  Timer and Siganl
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <signal.h>

#define CLOCKID	CLOCK_REALTIME

static void handle_alarm(int s)
{
	printf("received signal %d\n", s);
}


static int install_handler(int signum, void (*handler)(int))
{
	struct sigaction action;
	sigset_t mask;

	/* Unblock the signal. */
	sigemptyset(&mask);
	sigaddset(&mask, signum);
	sigprocmask(SIG_UNBLOCK, &mask, NULL);

	/* Install the signal handler. */
	action.sa_handler = handler;
	action.sa_flags = 0;
	sigemptyset(&action.sa_mask);
	sigaction(signum, &action, NULL);

	return 0;
}

static void usage(char *progname)
{
	fprintf(stderr,
		"usage: %s [options]\n"
		" -a val     request a one-shot alarm after 'val' seconds\n"
		" -A val     request a periodic alarm every 'val' seconds\n"
		" -g         get the ptp clock time\n"
		" -h         prints this message\n",
		progname);
}

int main(int argc, char *argv[])

{
	struct itimerspec timeout;
	struct timespec ts;
	struct sigevent sigevent;
	
	static timer_t timerid;
	int c;
	int gettime = 0;
	int oneshot = 0;
	int periodic = 0;
	char *progname;
	
	progname = strrchr(argv[0], '/');
	progname = progname ? 1+progname : argv[0];


	while (EOF != (c = getopt(argc, argv, "a:A:gh"))) {
			switch (c) {
			case 'a':
				oneshot = atoi(optarg);
				break;
			case 'A':
				periodic = atoi(optarg);
				break;
			case 'g':
				gettime = 1;
				break;
			case 'h':
				usage(progname);
				return 0;
			case '?':
			default:
				usage(progname);
				return -1;
			}
		}


	if (gettime) {
		if (clock_gettime(CLOCKID, &ts)) {
			printf("%d:%s	clock_gettime", __LINE__, __FUNCTION__);
		} else {
			printf("clock time: %ld.%09ld or %s",
			       ts.tv_sec, ts.tv_nsec, ctime(&ts.tv_sec));
		}
	}

	if (oneshot) {
		install_handler(SIGALRM, handle_alarm);
		/* Create a timer. */
		sigevent.sigev_notify = SIGEV_SIGNAL;
		sigevent.sigev_signo = SIGALRM;
		if (timer_create(CLOCKID, &sigevent, &timerid)) {
			printf("%d:%s	Failed to timer_create\n", __LINE__, __FUNCTION__);
			return -1;
		}
		printf("%d:%s	timer ID is 0x%1x\n", __LINE__, __FUNCTION__, (long) timerid);
		
		/* Start the timer. */
		memset(&timeout, 0, sizeof(timeout));
		timeout.it_value.tv_sec = oneshot;
		timeout.it_value.tv_nsec = 0;
		timeout.it_interval.tv_sec = 0;
		timeout.it_interval.tv_nsec = 0;

		
		if (timer_settime(timerid, 0, &timeout, NULL)) {
			printf("%d:%s	Error: timer_settime\n", __LINE__, __FUNCTION__);
			return -1;
		}
		
		pause();
		timer_delete(timerid);
	}
	
	if (periodic) {
		install_handler(SIGALRM, handle_alarm);
		/* Create a timer. */
		sigevent.sigev_notify = SIGEV_SIGNAL;
		sigevent.sigev_signo = SIGALRM;
		if (timer_create(CLOCKID, &sigevent, &timerid)) {
			printf("%d:%s	Failed to timer_create\n", __LINE__, __FUNCTION__);
			return -1;
		}
		/* Start the timer. */
		memset(&timeout, 0, sizeof(timeout));
		timeout.it_interval.tv_sec = periodic;
		timeout.it_value.tv_sec = periodic;
		if (timer_settime(timerid, 0, &timeout, NULL)) {
			printf("%d:%s	Error: timer_settime\n", __LINE__, __FUNCTION__);
			return -1;
		}
		while (1) {
			pause();
		}
		timer_delete(timerid);
	}

	return 0;
}
