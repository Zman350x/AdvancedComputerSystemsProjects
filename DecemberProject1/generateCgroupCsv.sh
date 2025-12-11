#!/bin/bash

# DISCLOSURE: SCRIPT MOSTLY GENERATED WITH CHATGPT

mkdir -p csvs
ulimit -s unlimited

TRIALS=10
OUTPUT_CSV="csvs/cgroups.csv"

# Write CSV header
echo "cgroup,duration,cycles" > "$OUTPUT_CSV"

# Loop over programs and trials
for ((i=1; i<=TRIALS; i++)); do
    echo "Running trial $i..."
    echo "$(sudo output/cgroups)" >> "$OUTPUT_CSV"
done
