#include "experiment-utils.h"
#include <cstddef>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

#define SHARED_OBJ "/ipc_shared_mem"
#define BUFFER_SIZE 65536

// shared data struct
typedef struct
{
    volatile bool isFull;
    volatile size_t head;
    volatile size_t tail;
    volatile int buf[BUFFER_SIZE];
} CircularBuffer;

void writeToCircBuf(CircularBuffer *circBuf, int val)
{
    if (circBuf->isFull)
        return;

    circBuf->buf[circBuf->head] = val;
    circBuf->head += 1; // Apparantly `++` is deprecated on volatile variables

    if (circBuf->head >= BUFFER_SIZE)
        circBuf->head -= BUFFER_SIZE;

    if (circBuf->head == circBuf->tail)
        circBuf->isFull = true;
}

int readFromCircBuf(CircularBuffer *circBuf)
{
    if (circBuf->head == circBuf->tail && !circBuf->isFull) // is empty
        return -1;

    int val = circBuf->buf[circBuf->tail];
    circBuf->tail += 1;

    if (circBuf->tail >= BUFFER_SIZE)
        circBuf->tail -= BUFFER_SIZE;

    circBuf->isFull = false;

    return val;
}

int main(int argc, char **argv)
{
    performanceSetup();

    int fd = shm_open(SHARED_OBJ, O_CREAT | O_RDWR, 0666);
    ftruncate(fd, sizeof(CircularBuffer));

    CircularBuffer *circBuf = (CircularBuffer*) mmap(nullptr,
                                                     sizeof(CircularBuffer),
                                                     PROT_READ | PROT_WRITE,
                                                     MAP_SHARED, fd, 0);
    close(fd);

    circBuf->isFull = false;
    circBuf->head = 0;
    circBuf->tail = 0;

    pid_t pid = fork();

    // Parent process
    if (pid != 0)
    {
        auto startTime = std::chrono::high_resolution_clock::now();
        auto startCycles = __rdtsc();

        for (int i = 0; i < MSG_COUNT; i++)
        {
            // Only write when there's room in the buffer
            while (circBuf->isFull) { }

            writeToCircBuf(circBuf, i);
        }

        std::cout << "Start time: " << startTime << " | "
                     "Start cycles: " << startCycles << std::endl;
    }

    // Child process
    else
    {
        int val = -1;
        while (val != MSG_COUNT - 1)
        {
            val = readFromCircBuf(circBuf);
        }

        auto endCycles = __rdtsc();
        auto endTime = std::chrono::high_resolution_clock::now();

        std::cout << "End time: " << endTime << " | "
                     "End cycles: " << endCycles << std::endl;
    }

    shm_unlink(SHARED_OBJ);

    return 0;
}
