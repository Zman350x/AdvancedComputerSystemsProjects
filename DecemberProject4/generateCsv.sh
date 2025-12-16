#!/bin/bash

mkdir -p csvs
ulimit -s unlimited

TRIALS=10
OUTPUT_CSV="csvs/tree.csv"

# Write CSV header
echo "locking,job,threads,keys,duration,cycles" > "$OUTPUT_CSV"

# Loop over programs and trials
for ((i=1; i<=TRIALS; i++)); do
    echo "Running trial $i with coarse locking, 1 thread, and 10,000 keys..."
    echo "$(output/coarse 1 10000)" >> "$OUTPUT_CSV"
    echo "Running trial $i with coarse locking, 1 thread, and 100,000 keys..."
    echo "$(output/coarse 1 100000)" >> "$OUTPUT_CSV"
    echo "Running trial $i with coarse locking, 1 thread, and 1,000,000 keys..."
    echo "$(output/coarse 1 1000000)" >> "$OUTPUT_CSV"

    echo "Running trial $i with coarse locking, 2 threads, and 10,000 keys..."
    echo "$(output/coarse 2 10000)" >> "$OUTPUT_CSV"
    echo "Running trial $i with coarse locking, 2 threads, and 100,000 keys..."
    echo "$(output/coarse 2 100000)" >> "$OUTPUT_CSV"
    echo "Running trial $i with coarse locking, 2 threads, and 1,000,000 keys..."
    echo "$(output/coarse 2 1000000)" >> "$OUTPUT_CSV"

    echo "Running trial $i with coarse locking, 4 threads, and 10,000 keys..."
    echo "$(output/coarse 4 10000)" >> "$OUTPUT_CSV"
    echo "Running trial $i with coarse locking, 4 threads, and 100,000 keys..."
    echo "$(output/coarse 4 100000)" >> "$OUTPUT_CSV"
    echo "Running trial $i with coarse locking, 4 threads, and 1,000,000 keys..."
    echo "$(output/coarse 4 1000000)" >> "$OUTPUT_CSV"

    echo "Running trial $i with coarse locking, 8 threads, and 10,000 keys..."
    echo "$(output/coarse 8 10000)" >> "$OUTPUT_CSV"
    echo "Running trial $i with coarse locking, 8 threads, and 100,000 keys..."
    echo "$(output/coarse 8 100000)" >> "$OUTPUT_CSV"
    echo "Running trial $i with coarse locking, 8 threads, and 1,000,000 keys..."
    echo "$(output/coarse 8 1000000)" >> "$OUTPUT_CSV"

    echo "Running trial $i with coarse locking, 12 threads, and 10,000 keys..."
    echo "$(output/coarse 12 10000)" >> "$OUTPUT_CSV"
    echo "Running trial $i with coarse locking, 12 threads, and 100,000 keys..."
    echo "$(output/coarse 12 100000)" >> "$OUTPUT_CSV"
    echo "Running trial $i with coarse locking, 12 threads, and 1,000,000 keys..."
    echo "$(output/coarse 12 1000000)" >> "$OUTPUT_CSV"

    echo "Running trial $i with read/write locking, 1 thread, and 10,000 keys..."
    echo "$(output/readwrite 1 10000)" >> "$OUTPUT_CSV"
    echo "Running trial $i with read/write locking, 1 thread, and 100,000 keys..."
    echo "$(output/readwrite 1 100000)" >> "$OUTPUT_CSV"
    echo "Running trial $i with read/write locking, 1 thread, and 1,000,000 keys..."
    echo "$(output/readwrite 1 1000000)" >> "$OUTPUT_CSV"

    echo "Running trial $i with read/write locking, 2 threads, and 10,000 keys..."
    echo "$(output/readwrite 2 10000)" >> "$OUTPUT_CSV"
    echo "Running trial $i with read/write locking, 2 threads, and 100,000 keys..."
    echo "$(output/readwrite 2 100000)" >> "$OUTPUT_CSV"
    echo "Running trial $i with read/write locking, 2 threads, and 1,000,000 keys..."
    echo "$(output/readwrite 2 1000000)" >> "$OUTPUT_CSV"

    echo "Running trial $i with read/write locking, 4 threads, and 10,000 keys..."
    echo "$(output/readwrite 4 10000)" >> "$OUTPUT_CSV"
    echo "Running trial $i with read/write locking, 4 threads, and 100,000 keys..."
    echo "$(output/readwrite 4 100000)" >> "$OUTPUT_CSV"
    echo "Running trial $i with read/write locking, 4 threads, and 1,000,000 keys..."
    echo "$(output/readwrite 4 1000000)" >> "$OUTPUT_CSV"

    echo "Running trial $i with read/write locking, 8 threads, and 10,000 keys..."
    echo "$(output/readwrite 8 10000)" >> "$OUTPUT_CSV"
    echo "Running trial $i with read/write locking, 8 threads, and 100,000 keys..."
    echo "$(output/readwrite 8 100000)" >> "$OUTPUT_CSV"
    echo "Running trial $i with read/write locking, 8 threads, and 1,000,000 keys..."
    echo "$(output/readwrite 8 1000000)" >> "$OUTPUT_CSV"

    echo "Running trial $i with read/write locking, 12 threads, and 10,000 keys..."
    echo "$(output/readwrite 12 10000)" >> "$OUTPUT_CSV"
    echo "Running trial $i with read/write locking, 12 threads, and 100,000 keys..."
    echo "$(output/readwrite 12 100000)" >> "$OUTPUT_CSV"
    echo "Running trial $i with read/write locking, 12 threads, and 1,000,000 keys..."
    echo "$(output/readwrite 12 1000000)" >> "$OUTPUT_CSV"
done
