#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) 
{
    printf("hello world (pid:%d)\n", (int) getpid());
    int rc = fork();
    if (rc < 0) 
    {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) 
    {
        printf("hello, I am child (pid:%d)\n", (int) getpid());
        char *execargs[3];
        execargs[0] = strdup("wc");
        execargs[1] = strdup("exec.c");
        execargs[2] = NULL;
        execvp(execargs[0], execargs);
        printf("This shouldn't print!");
    } else {
        int wc = wait(NULL);
        printf("hello, I am parent of %d (wc:%d) (pid:%d)\n", rc, wc, (int) getpid());
    }
    return 0;
}
