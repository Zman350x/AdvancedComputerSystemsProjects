// CODE MODIFIED FROM: https://biendltb.github.io/tech/inter-process-communication-ipc-in-cpp/

#include "experiment-utils.h"
#include <iostream>
#include <unistd.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define NAMED_PIPE "./fifo_pipe"

int main(int argc, char **argv)
{
    performanceSetup();
    int ret;

    // create the named pipe (fifo) with permission
    mkfifo(NAMED_PIPE, 0666);

    pid_t pid = fork();

    // Parent process writes to pipe
    if (pid != 0)
    {
        sleep(1); // Add delay so reader starts up first, shouldn't affect timing data

        auto startTime = std::chrono::high_resolution_clock::now();
        auto startCycles = __rdtsc();

        int fd = open(NAMED_PIPE, O_WRONLY | O_NONBLOCK); // Nonblock makes it async
        struct pollfd pfd{ .fd = fd, .events = POLLOUT };

        for (int i = 0; i < MSG_COUNT; i++)
        {
            while (true)
            {
                ret = poll(&pfd, 1, -1);
                if (ret > 0)
                    break;
            }
            write(fd, &i, sizeof(i));
        }
        close(fd);

        std::cout << "Start time: " << startTime << " | "
                     "Start cycles: " << startCycles << std::endl;
    }

    // Child process reads from pipe
    else
    {
        int fd = open(NAMED_PIPE, O_RDONLY | O_NONBLOCK); // Nonblock makes it async
        struct pollfd pfd{ .fd = fd, .events = POLLIN };

        int val = -1;
        while (val != MSG_COUNT - 1)
        {
            ret = poll(&pfd, 1, -1);
            if (ret > 0)
            {
                read(fd, &val, sizeof(val));
            }
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

