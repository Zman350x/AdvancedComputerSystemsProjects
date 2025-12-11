#!/bin/bash

mkdir -p csvs
ulimit -s unlimited

TRIALS=10
OUTPUT_CSV="csvs/thp.csv"

# Write CSV header
echo "is_thp,duration,cycles" > "$OUTPUT_CSV"

# Loop over programs and trials
for ((i=1; i<=TRIALS; i++)); do
    echo "Running trial $i with THP enabled..."
    echo "$(output/thp_enabled)" >> "$OUTPUT_CSV"
    echo "Running trial $i with THP disabled..."
    echo "$(output/thp_disabled)" >> "$OUTPUT_CSV"
done
