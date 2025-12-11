#!/bin/bash

# DISCLOSURE: SCRIPT MOSTLY GENERATED WITH CHATGPT

mkdir -p csvs
ulimit -s unlimited

# List of programs to benchmark
declare -A PROGRAMS=(
    ["baseline"]="output/ipc_baseline"
    ["zero-copy"]="output/ipc_zero_copy"
    ["async"]="output/ipc_async"
)
TRIALS=10
OUTPUT_CSV="csvs/ipc.csv"

# Write CSV header
echo "program,start_time,end_time,start_cycles,end_cycles" > "$OUTPUT_CSV"

# Function to convert program output to CSV line
parse_output() {
    local name="$1"
    awk -v prog="$name" '
    /Start time:/ {
        match($0, /Start time: ([0-9\-:. ]+) \| Start cycles: ([0-9]+)/, m)
        start_time = m[1]
        start_cycles = m[2]
    }
    /End time:/ {
        match($0, /End time: ([0-9\-:. ]+) \| End cycles: ([0-9]+)/, m)
        end_time = m[1]
        end_cycles = m[2]
    }
    END {
        print prog "," start_time "," end_time "," start_cycles "," end_cycles
    }
    '
}

# Loop over programs and trials
for name in "${!PROGRAMS[@]}"; do
    prog_path="${PROGRAMS[$name]}"
    for ((i=1; i<=TRIALS; i++)); do
        echo "Running $name (trial $i)..."
        output=$("$prog_path")
        csv_line=$(echo "$output" | parse_output "$name")
        echo "$csv_line" >> "$OUTPUT_CSV"
    done
done
