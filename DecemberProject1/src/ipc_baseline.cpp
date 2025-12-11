// CODE MODIFIED FROM: https://biendltb.github.io/tech/inter-process-communication-ipc-in-cpp/

#include "experiment-utils.h"
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define NAMED_PIPE "./fifo_pipe"

int main(int argc, char **argv)
{
    performanceSetup();

    // create the named pipe (fifo) with permission
    mkfifo(NAMED_PIPE, 0666);

    pid_t pid = fork();

    // Parent process writes to pipe
    if (pid != 0)
    {
        auto startTime = std::chrono::high_resolution_clock::now();
        auto startCycles = __rdtsc();

        int fd = open(NAMED_PIPE, O_WRONLY);
        for (int i = 0; i < MSG_COUNT; i++)
        {
            write(fd, &i, sizeof(i));
        }
        close(fd);

        std::cout << "Start time: " << startTime << " | "
                     "Start cycles: " << startCycles << std::endl;
    }

    // Child process reads from pipe
    else
    {
        int fd = open(NAMED_PIPE, O_RDONLY);
        int val = -1;
        while (val != MSG_COUNT - 1)
        {
            read(fd, &val, sizeof(val));
        }
        close(fd);

        auto endCycles = __rdtsc();
        auto endTime = std::chrono::high_resolution_clock::now();

        std::cout << "End time: " << endTime << " | "
                     "End cycles: " << endCycles << std::endl;
    }

    unlink(NAMED_PIPE);

    return 0;
}

