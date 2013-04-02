/* sig_handler.c */
#include <stdio.h>
#include <signal.h>

void sig_handler(int signo)
{
    if (signo == SIGINT)
        printf("Received SIGINT\n...Smb pressed Ctrl+C or not =)\n");
    signal(SIGINT, SIG_DFL);
    kill(getpid(), SIGINT);
}

