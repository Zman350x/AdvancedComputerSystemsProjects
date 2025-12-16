#include "experiment-utils.h"
#include "bst.h"
#include <atomic>
#include <cstdint>
#include <mutex>
#include <random>
#include <shared_mutex>
#include <thread>
#include <vector>

#define OPERATION_COUNT 100000

std::atomic<bool> start{false};

enum class Job
{
    READ_ONLY,
    WRITE_ONLY,
    READ_WRITE
};

// READ/WRITE-LOCKED BST

class ReadWriteBst
{
public:
    void insert(uint32_t key, const char *const value);
    char *find(uint32_t key) const;
    void erase(uint32_t key);
    void print() const;
private:
    Bst::Node *root = nullptr;
    mutable std::shared_mutex mtx;
};

void ReadWriteBst::insert(uint32_t key, const char *const value)
{
    std::unique_lock lock(mtx); // Write lock
    root = Bst::insert(root, key, value);
}

char *ReadWriteBst::find(uint32_t key) const
{
    std::shared_lock lock(mtx); // Read lock
    Bst::Node *temp = Bst::find(root, key);

    if (temp == nullptr)
        return nullptr;

    return temp->value;
}

void ReadWriteBst::erase(uint32_t key)
{
    std::unique_lock lock(mtx); // Write lock
    root = Bst::erase(root, key);
}

void ReadWriteBst::print() const
{
    std::shared_lock lock(mtx); // Read lock
    Bst::printTree(root);
}

// THREADING

void worker(int id, ReadWriteBst &tree, int keyCount, uint32_t opCount, Job job)
{
    thread_local std::minstd_rand rng(id + std::random_device{}());
    std::uniform_int_distribution<uint32_t> keyDist(0, keyCount - 1);
    std::uniform_int_distribution<uint32_t> chanceDist(0, 9);

    volatile char *read_data;

    while (!start.load(std::memory_order::acquire))
        std::this_thread::yield();

    switch (job)
    {
        case Job::WRITE_ONLY:
            for (uint32_t i = 0; i < opCount; ++i)
                tree.insert(keyDist(rng), "Hello World!");
            break;
        case Job::READ_ONLY:
            for (uint32_t i = 0; i < opCount; ++i)
                read_data = tree.find(keyDist(rng));
            break;
        case Job::READ_WRITE:
            for (uint32_t i = 0; i < opCount; ++i)
            {
                if (chanceDist(rng) < 7)
                    read_data = tree.find(keyDist(rng));
                else
                    tree.insert(keyDist(rng), "Hello World!");
            }
            break;
        default:
            break;
    }
}

// MAIN

int main(int argc, char **argv)
{
    performanceSetup();

    ReadWriteBst tree;

    if (argc < 3)
        return 1;

    int threadCount = atoi(argv[1]);
    int keyCount = atoi(argv[2]);

    uint32_t opsPerThread = OPERATION_COUNT / threadCount;

    std::vector<std::thread> threads;
    threads.reserve(threadCount);

    // WRITE-ONLY TEST

    for (int i = 0; i < threadCount; ++i)
        threads.emplace_back(worker, i, std::ref(tree), keyCount, opsPerThread, Job::WRITE_ONLY);

    auto startTime = std::chrono::high_resolution_clock::now();
    auto startCycles = __rdtsc();

    start.store(true, std::memory_order_release);
    for (auto& t : threads) {
        t.join();
    }

    auto endCycles = __rdtsc();
    auto endTime = std::chrono::high_resolution_clock::now();
    timespanSec timeDiff = endTime - startTime;
    uint64_t cycles = endCycles - startCycles;

    std::cout << "read_write,write_only," << threadCount << ","<< keyCount << "," <<
                 timeDiff.count() << "," << cycles << std::endl;

    // RESET

    threads.clear();
    start.store(false, std::memory_order_release);

    // READ-ONLY TEST

    for (int i = 0; i < threadCount; ++i)
        threads.emplace_back(worker, i, std::ref(tree), keyCount, opsPerThread, Job::READ_ONLY);

    startTime = std::chrono::high_resolution_clock::now();
    startCycles = __rdtsc();

    start.store(true, std::memory_order_release);
    for (auto& t : threads) {
        t.join();
    }

    endCycles = __rdtsc();
    endTime = std::chrono::high_resolution_clock::now();
    timeDiff = endTime - startTime;
    cycles = endCycles - startCycles;

    std::cout << "read_write,read_only," << threadCount << ","<< keyCount << "," <<
                 timeDiff.count() << "," << cycles << std::endl;

    // RESET

    threads.clear();
    start.store(false, std::memory_order_release);

    // READ-WRITE TEST

    for (int i = 0; i < threadCount; ++i)
        threads.emplace_back(worker, i, std::ref(tree), keyCount, opsPerThread, Job::READ_WRITE);

    startTime = std::chrono::high_resolution_clock::now();
    startCycles = __rdtsc();

    start.store(true, std::memory_order_release);
    for (auto& t : threads) {
        t.join();
    }

    endCycles = __rdtsc();
    endTime = std::chrono::high_resolution_clock::now();
    timeDiff = endTime - startTime;
    cycles = endCycles - startCycles;

    std::cout << "read_write,read_write," << threadCount << ","<< keyCount << "," <<
                 timeDiff.count() << "," << cycles << std::endl;

    return 0;
}
