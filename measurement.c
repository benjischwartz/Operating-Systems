#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <fcntl.h>
#include <stdlib.h>

/*
 * Measure cost of system call and context switch.
 * 
 * System call: Repeatedly call simple system call (0-byte read), 
 * time how long, and divde by the number of iterations.
 * Use gettimeofday() or rdtsc.
 * 
 * Context switch: repeatedly measure cost of communication 
 * between processes, using pipes. Ensure context-switch happens
 * on the same processor with something like sched_setaffinity().
*/
#define NUM_ITERATIONS 100

void context_switch();

int main(int argc, char* argv[]) {
    printf("Timing syscall...\n");
    
    struct timeval t1;
    struct timeval t2;
    gettimeofday(&t1, NULL);
    
    for (int i = 0; i < NUM_ITERATIONS; ++i) {
        read(0, NULL, 0);
    }
    gettimeofday(&t2, NULL);
    double res = ((double)t2.tv_usec - t1.tv_usec) / NUM_ITERATIONS;
    printf("Time taken: %f microseconds\n", res);

    
    printf("Timing context switch...\n");
    gettimeofday(&t1, NULL);
    for (int i = 0; i < NUM_ITERATIONS; ++i) {
        context_switch();
    }
    gettimeofday(&t2, NULL);
    res = ((double)t2.tv_usec - t1.tv_usec) / NUM_ITERATIONS;
    printf("Time taken: %f microseconds\n", res);
}

void context_switch() 
{
    /* pipfd[0] = read end, pipefd[1] = write end 
     * on success: returns 0. Falure: -1
     * If a process reads from empty pipe, will remain blocked
     * until data written to it. when full, write will remain
     * blocked until read from.
     * */
    int pipefd[2];      /* pipfd[0] = read end, pipefd[1] = write end */
    pid_t pid;          /* Store forks return value */
    char buf;           /* store read chars */
    if (pipe(pipefd) == -1) 
    {
        perror("pipe");
        exit(1);
    }

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit(1);
    }
    else if (pid == 0)  /* child process */
    {
        /* Close unused write end */
        close(pipefd[1]); 

        /* Read from pipe */
        //printf("Child: reading from pipe...\n");
        while (read(pipefd[0], &buf, 1) > 0) 
        {
            //write (STDOUT_FILENO, &buf, 1);
        }
        
        /* Write to stdout */
        //printf("Child: Done reading!\n");

        /* Close read end */
        close(pipefd[0]);
        exit(EXIT_SUCCESS);
    }
    else /* parent process */
    {
        /* Close unused read end */
        close(pipefd[0]); 
        
        /* Write to pipe */
        //printf("Parent: writing to pipe...\n");
        char *str = "Secret message!\n";
        write(pipefd[1], str, strlen(str));

        /* Close write end */
        close(pipefd[1]);

        /* Wait for child */
        wait(NULL);
        //printf("Parent: Hello child!\n");
        return;
    }
}
