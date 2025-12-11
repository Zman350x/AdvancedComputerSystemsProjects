#include "experiment-utils.h"
#include <cstdint>
#include <iostream>
#include <fstream>
#include <chrono>
#include <csignal>
#include <unistd.h>
#include <vector>
#include <sys/wait.h>
#include <x86intrin.h>

static const int NUM_CHILDREN = 100;

// Useless CPU waster
void run_hog() {
    while (true) {
        asm volatile("" ::: "memory");
    }
}

// Timed CPU workload
void run_timed_work(std::string name)
{
    auto startTime = std::chrono::high_resolution_clock::now();
    auto startCycles = __rdtsc();

    volatile uint64_t x = 0;
    for (uint64_t i = 0; i < UINT32_MAX; ++i)
        x += i;

    auto endCycles = __rdtsc();
    auto endTime = std::chrono::high_resolution_clock::now();
    timespanSec timeDiff = endTime - startTime;
    uint64_t cycles = endCycles - startCycles;

    std::cout << name << "," << timeDiff.count() << "," << cycles << std::endl;
}

void move_pid_to_cgroup(const std::string &cgroup_path, pid_t pid)
{
    std::string path = cgroup_path + "/cgroup.procs";
    std::ofstream ofs(path);
    if (!ofs) {
        std::cerr << "ERROR: Cannot open " << path
                  << " (need root?)\n";
        exit(1);
    }
    ofs << pid;
    ofs.close();
}

void setup_cgroup(const std::string &path, int weight)
{
    std::string mkdir_cmd = "mkdir -p " + path;
    system(mkdir_cmd.c_str());

    std::string wpath = path + "/cpu.weight";
    std::ofstream ofs(wpath);
    if (!ofs) {
        std::cerr << "ERROR: Cannot write " << wpath
                  << " (CPU controller enabled?)\n";
        exit(1);
    }
    ofs << weight;
    ofs.close();
}

int main()
{
    pid_t pid = getpid();

    // Leave any group we may be in due to parent process
    move_pid_to_cgroup("/sys/fs/cgroup", pid);

    std::vector<pid_t> children;

    // Launch useless child processes
    for (int i = 0; i < NUM_CHILDREN; i++)
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            run_hog();
            return 0;
        }
        else if (pid > 0)
            children.push_back(pid);
        else
            return 1;
    }

    sleep(1); // Let the children get started

    // Make sure this happens after spawning the children
    performanceSetup();

    // Run test
    run_timed_work("baseline");

    // Join slow cgroup
    std::string slow_cg = "/sys/fs/cgroup/demo_slow";
    setup_cgroup(slow_cg, 10);
    move_pid_to_cgroup(slow_cg, pid);

    // Run test
    run_timed_work("slow");

    // Leave slow cgroup
    move_pid_to_cgroup("/sys/fs/cgroup", pid);

    // Join fast cgroup
    std::string fast_cg = "/sys/fs/cgroup/demo_fast";
    setup_cgroup(fast_cg, 1000);
    move_pid_to_cgroup(fast_cg, pid);

    // Run test
    run_timed_work("fast");

    // Leave fast cgroup
    move_pid_to_cgroup("/sys/fs/cgroup", pid);

    // Kill useless children
    for (pid_t c : children)
        kill(c, SIGKILL);

    // Reap zombies
    for (int i = 0; i < NUM_CHILDREN; i++)
        wait(NULL);

    return 0;
}

